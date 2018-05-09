#include<iostream>
#include<fstream>
#define instructionLength 32

using namespace std;

int printMenu()
{

    // register
    cout<<"Registers:"<<endl;
    cout<<"$0: "<<0<<endl;
    cout<<"$1: "<<0<<endl;
    cout<<"$2: "<<0<<endl;
    cout<<"$3: "<<0<<endl;
    cout<<"$4: "<<0<<endl;
    cout<<"$5: "<<0<<endl;
    cout<<"$6: "<<0<<endl;
    cout<<"$7: "<<0<<endl;
    cout<<"$8: "<<0<<endl;
    cout<<"$9: "<<0<<endl;
    cout<<endl;

    //data memory
    cout<<"Data memory:"<<endl;
    cout<<"0x00: "<<0<<endl;
    cout<<"0x04: "<<0<<endl;
    cout<<"0x08: "<<0<<endl;
    cout<<"0x0C: "<<0<<endl;
    cout<<"0x10: "<<0<<endl;
    cout<<endl;

    // IF ID
    cout<<"IF/ID :"<<endl;
    cout<<"PC\t\t"<<0<<endl;
    cout<<"Instruction\t"<<0<<endl;
    cout<<endl;

    // ID EX
    cout<<"ID/EX :"<<endl;
    cout<<"ReadData1\t"<<0<<endl;
    cout<<"ReadData2\t"<<0<<endl;
    cout<<"sign_ext\t"<<0<<endl;
    cout<<"Rs\t\t"<<0<<endl;
    cout<<"Rt\t\t"<<0<<endl;
    cout<<"Rd\t\t"<<0<<endl;
    cout<<"Control signals\t"<<0<<endl;
    cout<<endl;

    // EX MEM
    cout<<"EX/MEM :"<<endl;
    cout<<"ALUout\t\t"<<0<<endl;
    cout<<"WritteData\t"<<0<<endl;
    cout<<"Rt/Rd\t\t"<<0<<endl;
    cout<<"Control signals\t"<<0<<endl;
    cout<<endl;

    // MEM WB
    cout<<"MEM/WB :"<<endl;
    cout<<"ReadData\t"<<0<<endl;
    cout<<"ALUout\t\t"<<0<<endl;
    cout<<"Rt/Rd\t\t"<<0<<endl;
    cout<<"Control signals\t"<<0<<endl;
    cout<<endl;

}

int main()
{
    ifstream ifs;
    ifs.open("General.txt", ios::in);

    char input[instructionLength];
    for(int i=0;i<instructionLength;i++)
    {
        ifs>>input[i];
    }

    for(int i=0;i<instructionLength;i++)
    {
        cout<<input[i];
    }
    cout<<endl;

    ifs.close();

    printMenu();
}
