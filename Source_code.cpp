#include <bits/stdc++.h>
using namespace std;

//represent each minterm with its number and binary representation
struct t
{
    int number;
    string binary;
    bool taken = false;
};

//to be used in step of circling 2 ones
struct s2
{
    int term1, term2;
    string binary;
    bool taken = false;
};

//to be used in step of circling 4 ones
struct s3
{
    int term1, term2, term3, term4;
    string binary;
};


int main()
{
    //read from user the number of minterms
    cout << "*" << "Hello, This is a Three-Variable K-Map Logic Minimization program" << "*" << endl;

	int num;
	cout << "Please enter the number of min-terms: " << endl;
	cin >> num;


	//handling validation of number of minterms
	while (num < 0 || num > 8)
	{
		cout << "Please enter a valid number of min-terms. Enter a number between 0 and 8!" << endl;
		cin >> num;
	}

	//special case: when number of minterms is 0, then the function is f = 0 (all cell are 0s)
	if (num == 0)
    {
        cout << "K-map =" << endl;
        for(int i =0 ; i < 2 ; i++){
            for(int j = 0 ; j<4 ; j++)
                cout<<"0  ";
            cout<<endl;
        }
        cout<<"\nSimplest function f= 0\n";
    }

    //special case: when number of minterms is 8, then the function f = 1 (all cells are 1s)
    else if (num == 8)
    {
        cout << "K-map =" << endl;
        for(int i =0 ; i < 2 ; i++){
            for(int j = 0 ; j<4 ; j++)
                cout<<"1  ";
            cout<<endl;
        }
        cout<<"\nSimplest function f= 1\n";
    }


    //other cases
    else
    {
        vector<t> minterms (num);                  //to store the minterms
        unordered_map <int, bool> covered;         // to be used in evaluating the final result: it indicates whether each minterm is included in the result or not


        //read minterms from the user
         cout << "Please enter the min-terms: " << endl;

         for (int i = 0; i < num; i++)
         {
             cin >> minterms[i].number;

             //handling validation of minterms (must be between 0 and 7)
             while (minterms[i].number < 0 || minterms[i].number > 7)
             {
                 cout << "Please enter a valid min-term. Enter a number between 0 and 7!" << endl;
                 cin >> minterms[i].number;
             }

             covered[minterms[i].number] = false;

             //assign each minterm its binary representation
             if(minterms[i].number == 0) minterms[i].binary = "000";
             else if (minterms[i].number == 1) minterms[i].binary = "001";
             else if (minterms[i].number == 2) minterms[i].binary = "010";
             else if (minterms[i].number == 3) minterms[i].binary = "011";
             else if (minterms[i].number == 4) minterms[i].binary = "100";
             else if (minterms[i].number == 5) minterms[i].binary = "101";
             else if (minterms[i].number == 6) minterms[i].binary = "110";
             else if (minterms[i].number == 7) minterms[i].binary = "111";
         }


         //print the k-map
         cout << "\nK-map: \n" << endl;

         bool flag = false;

         for (int i = 0; i < 8; i++)
         {
             for (int j = 0; j < num; j++)
             {
                 if (i == 2 || i == 6)
                 {
                     if (minterms[j].number == (i + 1))
                     {
                         cout << 1 << "     ";
                         flag = true;
                         break;
                     }
                 }

                 else if (i == 3 || i == 7)
                 {

                     if (minterms[j].number == (i - 1))
                     {
                         cout << 1 << "     ";
                         flag = true;
                         break;
                     }
                 }

                 else
                 {
                     if (minterms[j].number == i)
                     {
                         cout << 1 << "     ";
                         flag = true;
                         break;
                     }
                 }
             }

             if (!flag)
             {
                 cout << 0 << "     ";
             }
                 if (i == 3)
                     cout << endl;

                 flag = false;
         }



         //grouping the minterms with respect to number of 1s in their binary representation
        vector <vector<t>> numOfOnes (4);
        for (auto y: minterms)
        {
            if (y.number == 0) numOfOnes[0].push_back(y);                                          //0 ones
            else if (y.number == 1 || y.number == 2 || y.number == 4) numOfOnes[1].push_back(y);   //1 one
            else if (y.number == 3 || y.number == 5 || y.number == 6) numOfOnes[2].push_back(y);   // 2 ones
            else if (y.number == 7) numOfOnes[3].push_back(y);                                     //3 ones
        }


        //circling possible 2 adjacent ones together to make a pair of ones
        vector <vector<s2>> matched_pairs1(3);
        for (int i =0 ; i <3 ; i++)
        {
            for(int j = 0 ; j<(int)numOfOnes[i].size() ; j++)
            {
                for(int k=0 ; k<(int)numOfOnes[i+1].size(); k++)
                {
                    int counter =0, idx;
                    for(int l =0 ; l<3 ; l++)
                        if(numOfOnes[i][j].binary.at(l) != numOfOnes[i+1][k].binary.at(l))  //if the same bit index is different
                            {
                                counter++;
                                if(counter == 1) idx = l; //store the index
                            }

                    //if the two minterms differ in only one bit, then circle these 2 minterms together
                    if(counter == 1)
                    {
                        numOfOnes[i][j].taken = true;
                        numOfOnes[i+1][k].taken = true;

                        //the new binary representation after combining the two minterms
                        string  y = numOfOnes[i][j].binary;
                        y.at(idx) = '-';

                        //add the combined 2 minterms for the next step
                        matched_pairs1[i].push_back({numOfOnes[i][j].number, numOfOnes[i+1][k].number, y});
                    }
                }
            }
        }

        //circling possible 4 adjacent ones together to make 2 pairs of ones
        vector<vector<s3>> matched_pairs2 (2);
        for (int i =0 ; i <2 ; i++)
        {
            for(int j = 0 ; j<(int)matched_pairs1[i].size() ; j++)
            {
                for(int k=0 ; k<(int)matched_pairs1[i+1].size(); k++)
                {
                    int counter =0, idx;
                    for(int l =0 ; l<3 ; l++)
                        if(matched_pairs1[i][j].binary.at(l) != matched_pairs1[i+1][k].binary.at(l)) //if the same bit index is different
                            {
                                counter++;
                                if(counter == 1) idx = l;
                            }

                    if(counter == 1)
                    {
                        matched_pairs1[i][j].taken = true;
                        matched_pairs1[i+1][k].taken = true;

                        //the new binary representation after combining the two minterms
                        string  y = matched_pairs1[i][j].binary;
                        y.at(idx) = '-';

                        //add the combined 4 minterms for the final step
                        matched_pairs2[i].push_back({matched_pairs1[i][j].term1, matched_pairs1[i][j].term2,
                                                     matched_pairs1[i+1][k].term1, matched_pairs1[i+1][k].term2, y});
                    }
                }
            }
        }


        //constructing the table of all prime implicants
        unordered_map <int, vector<string>> prime;  // for every minterm, which expression/binary representation include it

        //minterms not included in the next simplification step
        for (int i = 0; i<4 ; i++)
            for (auto x: numOfOnes[i])
                if(!x.taken)
                {
                    prime[x.number].push_back(x.binary);
                }

        //minterms not included in the next simplification step
        for (int i = 0; i<3 ; i++)
            for (auto x: matched_pairs1[i])
                if(!x.taken)
                {
                    prime[x.term1].push_back(x.binary);
                    prime[x.term2].push_back(x.binary);
                }


        for (int i = 0 ; i<2 ; i++)
            for (auto x: matched_pairs2[i])
            {
                prime[x.term1].push_back(x.binary);

                prime[x.term2].push_back(x.binary);

                prime[x.term3].push_back(x.binary);

                prime[x.term4].push_back(x.binary);
                break;
            }

        set <string> result;
        unordered_map <string, vector<int>> mapp;

        for(auto x: prime)
            for(auto y: x.second)
                mapp[y].push_back(x.first);

        for(auto x: prime)
            if (x.second.size() == 1)
                result.insert(x.second[0]);


        for(auto x:result)
            for(auto y: mapp[x])
                covered[y] = true;

        for(auto x: covered)
            if(!x.second)
            {
                result.insert(prime[x.first][0]);
                x.second = true;
            }


    //    for(auto x: result)
    //        cout<<x<<endl;

        string finall="";
        for(auto x: result)
        {
            string temp = "";
            if(x[0] == '0') temp+="A'";
            else if (x[0] == '1') temp+="A";

            if(x[1] == '0') temp+="B'";
            else if (x[1] == '1') temp+="B";

            if(x[2] == '0') temp+="C'";
            else if (x[2] == '1') temp+="C";

            finall+= temp + " + ";

        }

        finall = finall.substr(0, finall.size()-3);
        cout<<"\n\nSimplest function f= "<<finall<<"\n";


    /*   Test conditions used while working on the project   */

    //    for(auto x: prime)
    //    {
    //        for(auto y: x.second)
    //        {
    //            cout<<x.first<<"  "<<y<<endl;
    //        }
    //    }


    //    for (int i = 0 ; i<2 ; i++)
    //        for (auto x: matched_pairs2[i])
    //        {
    //            printf("%d-%d-%d-%d  ", x.term1, x.term2, x.term3, x.term4);
    //            cout<<x.binary<<endl;
    //        }


    //    for (int i = 0 ; i<4 ; i++)
    //    {
    //        for (auto x: numOfOnes[i])
    //            cout<<x.number<<"  "<<x.binary<<"  "<<x.taken<<endl;
    //    }
    //
    //    cout<<endl<<endl;
    //
    //    for (int i = 0 ; i<3 ; i++)
    //    {
    //        for(auto x: matched_pairs1[i])
    //        {
    //            cout<<x.term1<<"-"<<x.term2<<"   "<<x.binary<<"  "<<x. taken<<endl;
    //        }
    //
    //    }


    //    for (int i =0 ; i<4 ; i++)
    //    {
    //        cout<<i<<": ";
    //        for (auto x :numOfOnes[i])
    //        {
    //            cout<<"  "<<x<<"  ";
    //        }
    //        cout<<endl;
    //    }


    }




	return 0;
}
