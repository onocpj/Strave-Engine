#pragma once


#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <functional>


// definition of thread mess
#define UNDEF_THR_POOL		nullptr
#define UNDEF_THR		nullptr
#define UNDEF_TASK		nullptr
#define	UNDEF_THR_NUM		0
#define UNDEF_THR_STATE		-1

#define THR_ACTIVE		true


namespace Strave {

	namespace thread {

		// Typedefinitions
		typedef struct {

			unsigned int totalThreadNumber;
			unsigned int bottomLine;
			unsigned int requiredThreadNumber;

		} ProcessorInit_t;

		// Thread pool, ables to create thread pool for enigne
		class ThreadPool {
		public:
			static ThreadPool* s_Pool;

		private:
			typedef struct {

				bool		threadStatus;
				std::thread*	thread;

			} Thread_t;
			typedef std::function<void(void)> NoParamTask_t;

			class ThreadFlag {
			public:
				inline ThreadFlag(ThreadPool& thrPool, Thread_t& thr) 
					: m_ThreadPool(&thrPool), m_Thread(&thr)
				{
					thrPool.UpdateThreadpoolStatus(true);			// update status of working and available threads when thread finished task
					thrPool.UpdateThreadStatus(thr, THR_ACTIVE);	// update thread status
				}
				inline ~ThreadFlag() {

					m_ThreadPool->UpdateThreadStatus(*m_Thread, !THR_ACTIVE);	// update status of working and available threads when thread finished task
					m_ThreadPool->UpdateThreadpoolStatus(false);				// update thread status

				}

			private:
				ThreadPool*	m_ThreadPool;
				Thread_t*	m_Thread;

			}; friend ThreadFlag;

		public:
			~ThreadPool();

			/**
				Sends task passed as parameter to queue.
				task will be waiting in queue, until some of the threads will process it

				@param: task_f - task that will be send to queue
			*/
			void Enqueue(NoParamTask_t task);
			template<typename... Args_t> inline void Enqueue(std::function<void(const Args_t&... args)> task) {

				m_TaskContainer.push(std::move(task));

			}
			/**
				Returns thread id of the thread, at specific index in container.
				If index is out of range, returns 0

				@param: contIndex - index of thread in container
			*/
			std::thread::id GetThreadID(unsigned int contIndex) const;
			/**
				Returns specific thread status

				@param: thrID - status of this thread will be returned 
					0 = not active
					1 = active
			*/
			int GetThreadStatus(const std::thread::id& thrID);
			/**
				Safely deletes thread pool
			*/
			void Delete(void);
			/**
				Safely creates thread pool

				@param: procInit - initialization of processor threads 
					(total threads, minimum threads for engine, required number of threads)
			*/
			static ThreadPool* Create(ProcessorInit_t procInit);
			/**
				Sends function passed as parameter to mutex and all cores are locked, until task is finished

				@param: funcRef - reference of task, that will be send to mutex 
			*/
			static void SendToMutex(const NoParamTask_t& funcRef);

		protected:
			/**
				Updates number of available and active threads

				@param: thrWorking - if thread is working pass: true, otherwise pass: false
			*/
			void UpdateThreadpoolStatus(bool thrWorking);
			/**
				Updates thread working status

				@param: thr - working status will be applied for this thread
				@param: thrStatus - status of thread
					true = working
					false = not working
			*/
			void UpdateThreadStatus(Thread_t& thr, bool thrStatus);

		private:
			ThreadPool(ProcessorInit_t procInit);

			/**
				Starts requiered number of threads

				@param: thrNum - number of threads that will be created, if possible
			*/
			void StartThreads(unsigned int thrNum);
			/**
				Validates if requiered number of threads is possible to create

				@param: totalThrNum - total number of available threads
				@param: bottomLine - bottom line of requiered threads
				@param: reqthrNum - requiered number of threads
			*/
			static bool ValidateThreads(unsigned int totalThrNum, unsigned int bottomLine, unsigned int reqThrNum);

		protected:
			unsigned int	m_AvailableThreads;
			unsigned int	m_ActiveThreads;

		private:
			std::vector<Thread_t>		m_ThreadContainer;
			std::queue<NoParamTask_t>	m_TaskContainer;
			std::mutex			m_TaskContainerMutex;
			ProcessorInit_t			m_ThreadpoolInfo;

		};

	}

}
