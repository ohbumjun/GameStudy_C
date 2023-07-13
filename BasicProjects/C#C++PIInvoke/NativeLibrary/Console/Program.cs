// See https://aka.ms/new-console-template for more information
using ConsoleApplication.Models;
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

		[DllImport("NativeLibrary.dll")]
		public static extern void BuyShoe(Shoe shoe);

		[DllImport("NativeLibrary.dll")]
		public static extern Shoe CreateShoe(double shoeSize);


		static void Main(string[]args)
		{
			Shoe newShoe = CreateShoe(2);

			BuyShoe(newShoe);

			HelloWorld();
			Console.WriteLine(AddNumbers(1, 2));
			Console.WriteLine(LongerThan5("222"));
			Console.WriteLine(GetName());

			/*
			*/
			WishList wishList = new WishList("Life");

			wishList.Name = "Birthday";
			Console.WriteLine(wishList.Name);

			// wishList.AddItem("Lamp");
			// wishList.AddItem("Graphics Card");
			// wishList.AddItem("Chipotle Burrito Bowl");
			// wishList.RemoveItem("Lamp");
			// 
			// Console.WriteLine($"{wishList.Name}: {wishList.Count} items");

			// wishList.Print();

			Console.ReadLine();
		}
	}
}


