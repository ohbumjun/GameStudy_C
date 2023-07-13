// See https://aka.ms/new-console-template for more information
using System;
using System.Runtime.InteropServices;

namespace ConsoleApplication
{
	class Program
	{
		[DllImport("NativeLibrary.dll")]
		public static extern void HelloWorld();

		[DllImport("NativeLibrary.dll", EntryPoint = "Add")]
		public static extern int AddNumbers(int num1, int num2);

		[DllImport("NativeLibrary.dll", EntryPoint = "LengthGreaterThan5")]
		public static extern bool LongerThan5(string value);

		[DllImport("NativeLibrary.dll")]
		[return: MarshalAs(UnmanagedType.BStr)]
		public static extern string GetName();

		static void Main(string[]args)
		{
			HelloWorld();
			Console.WriteLine(AddNumbers(1, 2));
			Console.WriteLine(LongerThan5("222"));
			Console.WriteLine(GetName());
		}
	}
}


