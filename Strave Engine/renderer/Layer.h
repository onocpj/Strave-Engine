namespace Strave 
{
	class GameObject2D;
	class Layer 
	{
	public:
		static void Lower(const GameObject2D& obj);
		static void Rise(const GameObject2D& obj);
		static void SendToBack(const GameObject2D& obj);
		static void SendToFront(const GameObject2D& obj);
		static void Merge(const GameObject2D& obj, const GameObject2D*& objs);

	private:

	};
}