using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication
{
	// struct 안에 field 를 정의할 때, 그 순서는 c++ 에 정의한 것과 같아야 한다.
	[StructLayout(LayoutKind.Sequential)]
	public struct Shoe
	{
		public int id;

		[MarshalAs(UnmanagedType.BStr)]
		public string color;
	}
}
