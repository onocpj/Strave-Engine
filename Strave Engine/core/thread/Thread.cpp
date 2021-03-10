#include "Thread.h"

#include "../logger/Log.h"
#include "../Application.h"
#include "../datatypes/UDT.h"


namespace Strave {

	// Application extern instance. 
	// This instance is used to chceck application running state
	extern Application* StraveApplication;

	namespace thread {

		//========================================================================================
		//									class: ThreadPool
		//========================================================================================

		ThreadPool* ThreadPool::s_Pool;

		ThreadPool::ThreadPool(ProcessorInit_t procInit)
			: m_AvailableThreads(procInit.totalThreadNumber), m_ActiveThreads(UNDEF_UINT),
			m_ThreadContainer(UNDEF_STD_VECTOR),
			m_ThreadpoolInfo({procInit.totalThreadNumber, procInit.bottomLine, procInit.requiredThreadNumber})
		{
			m_ThreadpoolInfo.totalThreadNumber	-= MAIN_THREAD;
			m_AvailableThreads					-= MAIN_THREAD;

			StartThreads(m_ThreadpoolInfo.requiredThreadNumber);
		}
		ThreadPool::~ThreadPool() {

		}

		void ThreadPool::Enqueue(NoParamTask_t task) {

			m_TaskContainer.push(std::move(task)); // pushes new task to task container

		}

		std::thread::id ThreadPool::GetThreadID(unsigned int contIndex) const {

			// if passed index is in container range
			if ((m_ThreadpoolInfo.requiredThreadNumber - 1) >= contIndex) {
				return m_ThreadContainer.at(contIndex).thread->get_id(); // return id of the thread, at this index

			} else { // else inform about index out of range
				SV_CORE_WARN("GetThreadID(): selected thread id is out of range");

				return std::thread::id(); // return undefined thread ID (0)
			}

		}

		int ThreadPool::GetThreadStatus(const std::thread::id& thrID) {

			// iterate through whole container
			for (std::vector<Thread_t>::iterator iter = m_ThreadContainer.begin(); iter != m_ThreadContainer.end(); iter++) {
				
				// if thread id passed as parameter was found in the container
				if (thrID == iter->thread->get_id()) {
					return iter->threadStatus; // return this threads status

				}
			}

			return UNDEF_THR_STATE; // otherwise, return undefined thread status

		}

		void ThreadPool::Delete(void) {

			unsigned int t_ThreadFreedUpCheck = 0; // variable that is used to monitor, if all of the threads were successfully deleted

			// if thread pool instance is defined
			if (this != UNDEF_PTR) {

				try {
					// cycle whole container
					for (unsigned int index = 0; index != m_ThreadpoolInfo.requiredThreadNumber; index++) {
						
						// if thread at specific index is joinable
						if (m_ThreadContainer.at(index).thread->joinable()) {
							m_ThreadContainer.at(index).thread->join();			// join this thread
								
							delete m_ThreadContainer.at(index).thread;			// delete this thread					
							m_ThreadContainer.at(index).thread = UNDEF_THR;		// null memory

							SV_CORE_INFO("Thread: joined and exited");

							t_ThreadFreedUpCheck++;								// freed up thread chceck passed

						} else { // if thread at specific index is not joinable, inform about that
							SV_CORE_WARN("Thread not joinable");
						}
					}

					// If all thread chcecks passed
					if (t_ThreadFreedUpCheck == m_ThreadpoolInfo.requiredThreadNumber) {
						delete this; // delete thread pool instance

						SV_CORE_INFO("ThreadPool::Delete(): Thread pool deleted");

					} else { // if all thread chcecks did not passed, inform about that
						SV_CORE_INFO("ThreadPool::Delete(): Unable to delete thread pool");
					}

				} catch (std::exception exception) {
					SV_CORE_ERROR(exception.what());
				}

			} else { // if thread pool instance is not defined, inform about it
				SV_CORE_WARN("ThreadPool::Delete(): Trying to delete undefined instance");
			}

		}

		ThreadPool* ThreadPool::Create(ProcessorInit_t procInit) {

			// if creation of required number of threads passed validation
			if (ValidateThreads(procInit.totalThreadNumber, procInit.bottomLine, procInit.requiredThreadNumber) == true) {
				SV_CORE_INFO("Thread pool created");

				return new ThreadPool(procInit); // create new instance of thread pool

			} else { // if creation of required number of threads has not passed, inform about that
				SV_CORE_WARN("Unable to create thread pool");
			}

			return UNDEF_THR_POOL; // return undefined thread pool as default value

		}

		void ThreadPool::SendToMutex(const NoParamTask_t& funcRef) {

			std::mutex t_Mutex;
			std::unique_lock<std::mutex> t_Locker{ t_Mutex };

			funcRef();

			t_Locker.unlock();

		}

		void ThreadPool::StartThreads(unsigned int thrNum) {

			for (unsigned int index = 0; index != m_ThreadpoolInfo.requiredThreadNumber; index++) {

				Thread_t t_thread = { 
					!THR_ACTIVE,				// passing thread status to initialization list
					new std::thread([=]() {		// passing new instance of thread to initialization list

						// while application exit state is not invoked
						while (!StraveApplication->GetOnExitState()) {

							// make sure mutex is locked to ensure that multiple threads will not access 
							// container of tasks at the same time and pick the same task
							std::unique_lock<std::mutex> t_locker{ m_TaskContainerMutex };

							// check if there is any task in queue in container
							if (!m_TaskContainer.empty()) {

								NoParamTask_t t_task;
								t_task = std::move(m_TaskContainer.front());					// if there is any task in queue, pick this task

								m_TaskContainer.pop();											// remove task from container so other threads will not be able to pick up the same task
								t_locker.unlock();												// unlock task container for other threads

								ThreadFlag t_ThreadRise(*this, m_ThreadContainer.at(index));	// rise flag that represents: new thread from thread pool is active
								t_task();														// start task

							}

						}

					})
				};

				m_ThreadContainer.push_back(std::move(t_thread)); // move thread instance to the thread container

			}

		}

		void ThreadPool::UpdateThreadpoolStatus(bool thrWorking) {

			if (thrWorking == true) {
				m_ActiveThreads++;
				m_AvailableThreads--;

			} else {
				m_ActiveThreads--;
				m_AvailableThreads++;
			}

		}

		void ThreadPool::UpdateThreadStatus(Thread_t& thr, bool thrStatus) {

			thr.threadStatus = thrStatus;

		}

		bool ThreadPool::ValidateThreads(unsigned int totalThrNum, unsigned int bottomLine, unsigned int reqThrNum) {

			if (reqThrNum <= totalThrNum) {
				if (reqThrNum >= bottomLine) {
					SV_CORE_INFO("Required number of threads validated");

					return true;

				} else {
					SV_CORE_WARN("Required number of threads is under bottom line");
				}

			} else {
				SV_CORE_WARN("Required number of threads is more than total amount of available cores");
			}
			
			return false;

		}

	}

}