/*******************
§@ªÌ:±ç¤¤Ãv
********************/
#include<iostream>
#include<fstream>
#define instructionLength 32

using namespace std;

class pipe
{
    pipe()
    {

    }

    int`Registers[10];
    int Memory[5];

    // IF/ID
    int pc;
    char instruction[instructionLength];

    // EX/MEM
    int ReadData1;
    int ReadData2;
    int sign_ext;
    int Rs;
    int Rt;
    int Rd;
    char ControlSignals1[9]; // for ID/EX

    // EX/MEM
    int ALUout1;
    int WriteData;
    int RtOrRd1; // for EX/MEM
    char ControlSignals2[5]; // for EX/MEM

    // MEM/WB
    int ReadData3; // for MEM/WB
    int ALUout2; // for MEM/WB
    int RtOrRd2; // for MEM/WB
    char ControlSignals3[5]; // for MEM/WB

    int IF(char ins[])
    {
        pc+=4;
        for(int i=0;i<instructionLength;i++)
            instruction[i]=ins[i];
    }

    int ID()
    {
        // opcode decode
        int zero_detect=1;
        int ControlSignals1_temp[9]={0};
        for(int i=0;i<6;i++)
            if(instruction[i]-'0'!=0)
                zero_detect=0;
        if(zero_detect) // if it is r type
        {
            ControlSignals1_temp[0]=1;
            ControlSignals1_temp[1]=1;
            ControlSignals1_temp[2]=0;
            ControlSignals1_temp[3]=0;
            ControlSignals1_temp[4]=0;
            ControlSignals1_temp[5]=0;
            ControlSignals1_temp[6]=0;
            ControlSignals1_temp[7]=1;
            ControlSignals1_temp[8]=0;
        }


        //$rs decode
        int rs_temp=0;
        {
            int 2pow_temp=1;
            for(int i=10;i>=6;i--)
            {
                int temp=instruction[i]-'0';
                temp*=2pow_temp;
                rs_temp+=temp;
                2pow_temp*=2;
            }
        }

        //$rt decode
        int rt_temp=0;
        {
            int 2pow_temp=1;
            for(int i=10;i>=6;i--)
            {
                int temp=instruction[i]-'0';
                temp*=2pow_temp;
                rt_temp+=temp;
                2pow_temp*=2;
            }
        }

        //$rd decode
        int rd_temp=0;
        {
            int 2pow_temp=1;
            for(int i=10;i>=6;i--)
            {
                int temp=instruction[i]-'0';
                temp*=2pow_temp;
                rd_temp+=temp;
                2pow_temp*=2;
            }
        }
        //funct decode
        //this will pass by sign_ext to EXE
        //this will decode at EXE
        //pass by char array


        //immediate decode
        //load store immediate
        int ImmLS_temp=0;
        {
            int 2pow_temp=1;
            for(int i=10;i>=6;i--)
            {
                int temp=instruction[i]-'0';
                temp*=2pow_temp;
                ImmLS_temp+=temp;
                2pow_temp*=2;
            }
        }
        //branch immediate
        int ImmBr_temp=0;
        {
            int 2pow_temp=1;
            for(int i=10;i>=6;i--)
            {
                int temp=instruction[i]-'0';
                temp*=2pow_temp;
                ImmBr_temp+=temp;
                2pow_temp*=2;
            }
        }
        ImmBr_temp*=4;
    }

    int EXE()
    {
        int tempA=Rs;
        int tempB;
        if(ControlSignals1[3]) //ALUSrc
            tempB=sign_ext;
        else
            tempB=Rt;

        // r type
        // determine the ALUctr
        if(ControlSignals1[1]==1&&ControlSignals1[2]==0)
        {

        }

        // i type lw/sw
        // determine the ALUctr

        // i type beq
        // determine the ALUctr

    }

};

int printMenu(pipe a)
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

    string forTest;
    /*char input[instructionLength];
    for(int i=0;i<instructionLength;i++)
    {
        ifs>>input[i];
    }

    for(int i=0;i<instructionLength;i++)
    {
        cout<<input[i];
    }
    cout<<endl;*/

    ifs>>forTest;
    cout<<forTest;

    ifs.close();

    printMenu();
}
