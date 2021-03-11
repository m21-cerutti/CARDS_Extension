
namespace Utility.Singleton
{
	/// <summary>
	/// Base class for singleton.
	/// </summary>
	/// <typeparam name="T"> The class to apply the singleton pattern.</typeparam>
	public class Singleton<T> where T : class, new()
	{
		private static T _instance = null;
		public T Instance
		{
			get
			{
				if (_instance == null)
				{
					_instance = new T();
				}

				return _instance;
			}
		}
	}
}