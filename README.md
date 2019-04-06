# dryagin7302_sem4_lab3

//file description
Structs.h contains structs Node, List and methods of this structs/
Useful.h cintains functions to realize methods of Structs.h



//The visualization of the algorithm and examples

//Example message is "Veni, vidi, vici!"
//list.output_vertically() informative fields are located: slash(item:frequency:code)
//slash points to the parent of the node
//Result looks like:
            /(v:2:111)
        /(dv:3:11)
            \(d:1:110)
    /( ,dv:7:1)
            /(,:2:101)
        \( ,:4:10)
            \( :2:100)
(in!ceV ,dv:17:)
                /(V:1:0111)
            /(eV:2:011)
                \(e:1:0110)
        /(!ceV:4:01)
                /(c:1:0101)
            \(!c:2:010)
                \(!:1:0100)
    \(in!ceV:10:0)
            /(n:1:001)
        \(in:6:00)
            \(i:5:000)
 

//list.output_vertically() informative fields are located: (item:frequency:code)
//the function works correctly with a small number of characters due to the width of the console
//Example message is "wow, looks cool"
//Result looks like:
                                                                         ( ,csklow:15:)

                                  ( ,cskl:8:0)                                                                      (ow:7:1)

               ( ,cs:5:00)                               (kl:3:01)                                (o:5:10)                                (w:2:11)              

     ( ,:3:000)          (cs:2:001)           (k:1:010)            (l:2:011)

( :2:0000)(,:1:0001)(c:1:0010)(s:1:0011)


//Example of main function of coding and decoding message:
int main()
{
	system("mode 160,30");

	List list;
	char* mes = "Nothing emboldens sin so much as mercy."; //Shakespeare
	list.get_list(mes);
	list.Huffman();
  
	cout << mes << endl;

	cout << endl << "Coded message: "<< endl;
	bool* cod = list.code_message(mes);
	for (int i = 0; i < list.coded_len; i++)
		cout << *(cod + i);
	cout << endl;

	cout << endl << "Decoded message: " << endl;
	cout << list.decode_message(cod) << endl;

	system("pause");
	return 0;
}
