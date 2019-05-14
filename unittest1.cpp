#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Проект2/Huffman.h"
#include "../Проект2/Useful.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:


		TEST_METHOD(Shakespear)
		{
			Huffman a;
			char* message = "Nothing emboldens sin so much as mercy.";
			Assert::AreEqual(0, string_compare(message, a.decode_message(a.code_message(message))));
		}
		
		TEST_METHOD(Lorem_ipsum)
		{
			Huffman a;
			char *message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam in velit non augue luctus maximus. Praesent scelerisque sem justo, a volutpat mi semper laoreet. Aliquam sed diam nulla. Curabitur quis justo sit amet erat lobortis faucibus eu at orci. Ut sed sodales elit, non blandit nisl. Nulla id aliquam lectus, in placerat sem. Praesent in.";
			Assert::AreEqual(0, string_compare(message, a.decode_message(a.code_message(message))));
		}

		TEST_METHOD(caesar)
		{
			Huffman a;
			char *message = "Veni, vidi, vici";
			Assert::AreEqual(0, string_compare(message, a.decode_message(a.code_message(message))));
		}

		TEST_METHOD(one_letter)
		{
			Huffman a;
			char *message = "a";
			Assert::AreEqual(0, string_compare(message, a.decode_message(a.code_message(message))));
		}

		TEST_METHOD(one_letter_multiple)
		{
			Huffman a;
			char *message = "aaaa";
			Assert::AreEqual(0, string_compare(message, a.decode_message(a.code_message(message))));
		}

		TEST_METHOD(two_letter)
		{
			Huffman a;
			char *message = "abaababaa";
			Assert::AreEqual(0, string_compare(message, a.decode_message(a.code_message(message))));
		}

		TEST_METHOD(empty_string)
		{
			Huffman a;
			try
			{
				a.code_message("");
			}
			catch (invalid_argument e)
			{
				Assert::AreEqual(e.what(), "Message is empty");
			}
		}

		TEST_METHOD(null_string)
		{
			Huffman a;
			try
			{
				a.code_message(nullptr);
			}
			catch (invalid_argument e)
			{
				Assert::AreEqual(e.what(), "Message is empty");
			}
		}
	};
}
