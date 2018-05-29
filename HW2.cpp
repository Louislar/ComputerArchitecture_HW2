/*******************
§@ªÌ:±ç¤¤Ãv
********************/
#include<iostream>
#include<fstream>
#define instructionLength 32

using namespace std;

class pipe
{
public:

    pipe()
    {
        Registers[0]=0;
        Registers[1]=9;
        Registers[2]=5;
        Registers[3]=7;
        Registers[4]=1;
        Registers[5]=2;
        Registers[6]=3;
        Registers[7]=4;
        Registers[8]=5;
        Registers[9]=6;
        Memory[0]=5;
        Memory[1]=9;
        Memory[2]=4;
        Memory[3]=8;
        Memory[4]=7;

        // IF/ID
        pc=0;
        for(int i=0;i<instructionLength;i++)
            instruction[i]='0';

        // ID/EX
        ReadData1=0;
        ReadData2=0;
        sign_ext=0;
        Rs=0;
        Rt=0;
        Rd=0;
        for(int i=0;i<9;i++)
            ControlSignals1[i]=0; // for ID/EX
        funct=0; // i'm too lazy to decode this at EX

        // EX/MEM
        ALUout1=0;
        WriteData=0;
        RtOrRd1=0; // for EX/MEM
        for(int i=0;i<5;i++)
            ControlSignals2[i]=0; // for EX/MEM

        // MEM/WB
        ReadData3=0; // for MEM/WB
        ALUout2=0; // for MEM/WB
        RtOrRd2=0; // for MEM/WB
        for(int i=0;i<2;i++)
            ControlSignals3[i]=0; // for MEM/WB

    }
    int forwardingUnit(int& forwardingBitsA, int& forwardingBitsB)
    {
        // EX hazard
        if(ControlSignals2[3]==1) // if EX/MEM.RegWrite==1
            if(RtOrRd1!=0)        // if EX/MEM.Rd!=0
            {
                // EX/MEM.Rd==ID/EXE.Rs
                if(RtOrRd1==Rs)
                    forwardingBitsA=2;

                // EX/MEM.Rd==ID/EXE.Rt
                if(RtOrRd1==Rt)
                    forwardingBitsB=2;
            }

        //MEM hazard
        if(ControlSignals3[0]==1) // if MEM/WB.RegWrite==1
            if(RtOrRd2!=0)        // if MEM/WB.Rd!=0
                if(RtOrRd1!=Rs)   // if not EX hazard
                {
                    // MEM/WB.Rd==ID/EXE.Rs
                    if(RtOrRd2==Rs)
                        forwadingBitsA=1;

                    // MEM/WB.Rd==ID/EXE.Rt
                    if(RtOrRd2==Rt)
                        forwardingBitsB=1;
                }



    }
    int nextCC(char ins[])
    {
        //take data from register and do some thing
        char IF_instruction_temp[instructionLength];
        int ID_temp[7];
        int ID_controlsignal1_temp[9];
        int EXE_temp[3];
        int EXE_controlsignal2_temp[5];
        int MEM_temp[3];
        int MEM_controlsignal3_temp[2];
        int forwardingBitsA=0;// use in decimal, not in binary for convenience
        int forwardingBitsB=0;

        forwardingUnit(forwardingBitsA, forwardingBitsB);

        // do EXE first, because we need ALUout for
        // data hazard forwarding
        EXE(EXE_temp, EXE_controlsignal2_temp,
            forwardingBitsA, forwardingBitsB);

        IF(IF_instruction_temp, ins);
        ID(ID_temp, ID_controlsignal1_temp);
        MEM(MEM_temp, MEM_controlsignal3_temp);
        WB();

        //test
        cout<<"\niiiiiiiiiiiiiiiiiiiii: ";
        for(int i=0;i<instructionLength;i++)
            cout<<instruction[i];
        //test


        //Write all the data to the registers
        //IF/ID
        for(int i=0;i<instructionLength;i++)
            instruction[i]=IF_instruction_temp[i];

        //ID/EXE
        ReadData1=ID_temp[0];
        ReadData2=ID_temp[1];
        sign_ext=ID_temp[2];
        Rs=ID_temp[3];
        Rt=ID_temp[4];
        Rd=ID_temp[5];
        funct=ID_temp[6]; // i'm too lazy to decode this at EX
        for(int i=0;i<9;i++) ControlSignals1[i]=ID_controlsignal1_temp[i];

        //EXE/MEM
        ALUout1=EXE_temp[0];
        WriteData=EXE_temp[1];
        RtOrRd1=EXE_temp[2]; // for EX/MEM
        for(int i=0;i<5;i++) ControlSignals2[i]=EXE_controlsignal2_temp[i];

        //MEM/WB
        ReadData3=MEM_temp[0]; // for MEM/WB
        ALUout2=MEM_temp[1]; // for MEM/WB
        RtOrRd2=MEM_temp[2]; // for MEM/WB
        for(int i=0;i<2;i++) ControlSignals3[i]=MEM_controlsignal3_temp[i];
    }

    int Registers[10];
    int Memory[5];

    // IF/ID
    int pc;
    char instruction[instructionLength];

    // ID/EX
    int ReadData1;
    int ReadData2;
    int sign_ext;
    int Rs;
    int Rt;
    int Rd;
    int ControlSignals1[9]; // for ID/EX
    int funct; // i'm too lazy to decode this at EX

    // EX/MEM
    int ALUout1;
    int WriteData;
    int RtOrRd1; // for EX/MEM
    int ControlSignals2[5]; // for EX/MEM

    // MEM/WB
    int ReadData3; // for MEM/WB
    int ALUout2; // for MEM/WB
    int RtOrRd2; // for MEM/WB
    int ControlSignals3[2]; // for MEM/WB

    int IF(char instruction_temp[], char ins[])
    {
        pc+=4;
        for(int i=0;i<instructionLength;i++)
            instruction_temp[i]=ins[i];
    }

    int ID(int ID_temp[], int ID_controlsignal1_temp[])
    {
        /***************************opcode finish**************/
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

        int opcode2Integer=0;// change opcode into integer
        {
            int pow2_temp=1;
            for(int i=5;i>=0;i--)
            {
                opcode2Integer+=(instruction[i]-'0')*pow2_temp;
                pow2_temp*=2;
            }
        }

        cout<<"\ncode2Int: "<<opcode2Integer<<endl;

        if(opcode2Integer==35)// if opcode== lw
        {
            ControlSignals1_temp[0]=0;
            ControlSignals1_temp[1]=0;
            ControlSignals1_temp[2]=0;
            ControlSignals1_temp[3]=1;
            ControlSignals1_temp[4]=0;
            ControlSignals1_temp[5]=1;
            ControlSignals1_temp[6]=0;
            ControlSignals1_temp[7]=1;
            ControlSignals1_temp[8]=1;
        }

        if(opcode2Integer==43)// if opcode== sw
        {
            ControlSignals1_temp[0]=0;
            ControlSignals1_temp[1]=0;
            ControlSignals1_temp[2]=0;
            ControlSignals1_temp[3]=1;
            ControlSignals1_temp[4]=0;
            ControlSignals1_temp[5]=0;
            ControlSignals1_temp[6]=1;
            ControlSignals1_temp[7]=0;
            ControlSignals1_temp[8]=0;
        }
        if(opcode2Integer==4)// if opcode== beq
        {
            ControlSignals1_temp[0]=0;
            ControlSignals1_temp[1]=0;
            ControlSignals1_temp[2]=1;
            ControlSignals1_temp[3]=0;
            ControlSignals1_temp[4]=1;
            ControlSignals1_temp[5]=0;
            ControlSignals1_temp[6]=0;
            ControlSignals1_temp[7]=0;
            ControlSignals1_temp[8]=0;
        }
        if(opcode2Integer==8)// if opcode== addi
        {
            ControlSignals1_temp[0]=0;//RegDst
            ControlSignals1_temp[1]=0;//ALUop1
            ControlSignals1_temp[2]=0;//ALUop0
            ControlSignals1_temp[3]=1;//ALUsrc
            ControlSignals1_temp[4]=0;//Branch
            ControlSignals1_temp[5]=0;//MemRead
            ControlSignals1_temp[6]=0;//MemWrite
            ControlSignals1_temp[7]=1;//RegWrite
            ControlSignals1_temp[8]=0;//MemToReg
        }
        if(opcode2Integer==12)// if opcode== andi
        {
            ControlSignals1_temp[0]=0;
            ControlSignals1_temp[1]=1;
            ControlSignals1_temp[2]=1;
            ControlSignals1_temp[3]=1;
            ControlSignals1_temp[4]=0;
            ControlSignals1_temp[5]=0;
            ControlSignals1_temp[6]=0;
            ControlSignals1_temp[7]=1;
            ControlSignals1_temp[8]=0;
        }

        //$rs decode
        int rs_temp=0;
        {
            int pow2_temp=1;
            for(int i=10;i>=6;i--)
            {
                int temp=instruction[i]-'0';
                temp*=pow2_temp;
                rs_temp+=temp;
                pow2_temp*=2;
            }
        }
        //not necessary now
        //convert address into real value
        //rs_temp=Registers[rs_temp];

        //$rt decode
        int rt_temp=0;
        {
            int pow2_temp=1;
            for(int i=15;i>=11;i--)
            {
                int temp=instruction[i]-'0';
                temp*=pow2_temp;
                rt_temp+=temp;
                pow2_temp*=2;
            }
        }
        //convert address into real value
        //rt_temp=Registers[rt_temp];

        //$rd decode
        int rd_temp=0;
        {
            int pow2_temp=1;
            for(int i=20;i>=16;i--)
            {
                int temp=instruction[i]-'0';
                temp*=pow2_temp;
                rd_temp+=temp;
                pow2_temp*=2;
            }
        }
        //convert address into real value
        //rd_temp=Registers[rd_temp];

        //funct decode
        //this will pass by sign_ext to EXE
        //this will decode into ALUctr at EXE
        int funct_temp=0;
        {
            int pow2_temp=1;
            for(int i=31;i>=26;i--)
            {
                int temp=instruction[i]-'0';
                temp*=pow2_temp;
                funct_temp+=temp;
                pow2_temp*=2;
            }
        }

        //calculate sign_ext
        int sign_ext_temp=0;
        {
            int pow2_temp=1;
            for(int i=31;i>=16;i--)
            {
                int temp=instruction[i]-'0';
                temp*=pow2_temp;
                sign_ext_temp+=temp;
                pow2_temp*=2;
            }
        }



        //immediate decode
        //load store immediate
        //this equal to sign_ext
        int ImmLS_temp=0;
        {
            int pow2_temp=1;
            for(int i=31;i>=16;i--)
            {
                int temp=instruction[i]-'0';
                temp*=pow2_temp;
                ImmLS_temp+=temp;
                pow2_temp*=2;
            }
        }
        //branch immediate
        int ImmBr_temp=0;
        {
            int pow2_temp=1;
            for(int i=31;i>=16;i--)
            {
                int temp=instruction[i]-'0';
                temp*=pow2_temp;
                ImmBr_temp+=temp;
                pow2_temp*=2;
            }
        }
        ImmBr_temp*=4;

        /*******************branch undone*********************/
        /*****need to flush all the control signal to zero****/
        //branch
        //used to do at EXE, but now do it earlier
        //, at ID. 白話文:branch提早做
        if(ControlSignals1[4])// control signal "branch" == 1
        {
            if(rs_temp==rd_temp)// sure to jump
                pc=pc+ImmBr_temp;
        }


        ID_temp[0]=Registers[rs_temp];
        ID_temp[1]=Registers[rt_temp];
        ID_temp[2]=sign_ext_temp;
        ID_temp[3]=rs_temp;
        ID_temp[4]=rt_temp;
        ID_temp[5]=rd_temp;
        ID_temp[6]=funct_temp;
        for(int i=0;i<9;i++) ID_controlsignal1_temp[i]=ControlSignals1_temp[i];
    }// ID end

    int EXE(int EXE_temp[], int EXE_controlsignal2_temp[],
             int forwaringBitsA, int forwaringBitsB)
    {
        int ControlSignals2_temp[5];
        for(int i=0;i<5;i++) ControlSignals2_temp[i]=ControlSignals1[i+4];
        int ALUctr_temp=0;
        int tempA=ReadData1;
        int tempB;
        if(ControlSignals1[3]) //ALUSrc
            tempB=sign_ext;
        else
            tempB=ReadData2;

        // r type
        // determine the ALUctr
        if(ControlSignals1[1]==1&&ControlSignals1[2]==0)
        {
            if(funct==32)//func==100000 add
                ALUctr_temp=2;
            if(funct==34)//func==100010 sub
                ALUctr_temp=6;
            if(funct==36)//func==100100 and
                ALUctr_temp=0;
            if(funct==37)//func==100101 or
                ALUctr_temp=1;
            if(funct==41)//func==101010 slt
                ALUctr_temp=7;

        }

        // i type

        //ALUop == 11
        if(ControlSignals1[1]==1&&ControlSignals1[2]==1)
        {
            ALUctr_temp=0;// and
        }
        //ALUop == 00
        if(ControlSignals1[1]==0&&ControlSignals1[2]==0)
        {
            ALUctr_temp=2;// add
        }
        //ALUop == 01
        if(ControlSignals1[1]==0&&ControlSignals1[2]==1)
        {
            ALUctr_temp=6;// sub
        }

        //ALU execute
        int ALUout_temp;
        if(ALUctr_temp==0)//and
        {
            if(ControlSignals1[3])//ALUSrc==1
            {
                ALUout_temp=Registers[Rs]&sign_ext;
                if(forwaringBitsA==1)

                if(forwaringBitsA==2)
                    ALUout_temp=ALUout1&sign_ext;
            }

            else                  //ALUSrc==0
            {
                int temp01;//
                int temp02;
                ALUout_temp=Registers[Rs]&Registers[Rt];

            }
        }
        if(ALUctr_temp==1)//or
        {
            if(ControlSignals1[3])//ALUSrc==1
                ALUout_temp=Registers[Rs]|sign_ext;
            else                  //ALUSrc==0
                ALUout_temp=Registers[Rs]|Registers[Rt];
        }
        if(ALUctr_temp==2)//add
        {
            if(ControlSignals1[3])//ALUSrc==1
                ALUout_temp=Registers[Rs]+sign_ext;
            else                  //ALUSrc==0
                ALUout_temp=Registers[Rs]+Registers[Rt];
        }
        if(ALUctr_temp==6)//sub
        {
            if(ControlSignals1[3])//ALUSrc==1
                ALUout_temp=Registers[Rs]-sign_ext;
            else                  //ALUSrc==0
                ALUout_temp=Registers[Rs]-Registers[Rt];
        }
        if(ALUctr_temp==7)//slt
        {
            if(Rs<Rt)
                ALUout_temp=1;
            else
                ALUout_temp=0;
        }

        // determine RsOrRd by RegDst
        int RtOrRd1_temp;
        if(ControlSignals1[0])
        {
            RtOrRd1_temp=Rd;
        }
        else if(!ControlSignals1[0])
        {
            RtOrRd1_temp=Rt;
        }


        EXE_temp[0]=ALUout_temp;
        EXE_temp[1]=Registers[Rt];// write data
        EXE_temp[2]=RtOrRd1_temp;//RtOrRd1, this passes register's address
        for(int i=0;i<5;i++) EXE_controlsignal2_temp[i]=ControlSignals2_temp[i];

    }//EXE end


    /******************MEM finish***********************/
    int MEM(int MEM_temp[], int MEM_controlsignal3_temp[])
    {
        int ControlSignals3_temp[2];
        for(int i=0;i<2;i++) ControlSignals3_temp[i]=ControlSignals2[i+3];
        int ALUout2_temp=ALUout1;
        int ReadData3_temp=0;
        int RtOrRd2_temp=RtOrRd1;
        if(ControlSignals2[1])//if Mem Read == 1
        {
            //ALUout1 need to be adjusted
            int temp=ALUout1/4;
            ReadData3_temp=Memory[temp];
        }
        if(ControlSignals2[2])//if Mem Write == 1
        {
            int temp=ALUout1/4;
            Memory[temp]=WriteData;
        }


        MEM_temp[0]=ReadData3_temp;
        MEM_temp[1]=ALUout2_temp;
        MEM_temp[2]=RtOrRd2_temp;
        for(int i=0;i<2;i++) MEM_controlsignal3_temp[i]=ControlSignals3_temp[i];

    }// MEM end

    int WB()
    {
        int WriteBackData;
        if(ControlSignals3[1]) // if MemToReg == 1
        {
            WriteBackData=ReadData3;
        }
        else // if MemToReg == 0
        {
            WriteBackData=ALUout2;
        }

        if(ControlSignals3[0]) // if RegWrite == 1
            Registers[RtOrRd2]=WriteBackData;
    }// WB end


};

int printMenu(pipe a)
{

    // register
    cout<<"Registers:"<<endl;
    cout<<"$0: "<<a.Registers[0]<<endl;
    cout<<"$1: "<<a.Registers[1]<<endl;
    cout<<"$2: "<<a.Registers[2]<<endl;
    cout<<"$3: "<<a.Registers[3]<<endl;
    cout<<"$4: "<<a.Registers[4]<<endl;
    cout<<"$5: "<<a.Registers[5]<<endl;
    cout<<"$6: "<<a.Registers[6]<<endl;
    cout<<"$7: "<<a.Registers[7]<<endl;
    cout<<"$8: "<<a.Registers[8]<<endl;
    cout<<"$9: "<<a.Registers[9]<<endl;
    cout<<endl;

    //data memory
    cout<<"Data memory:"<<endl;
    cout<<"0x00: "<<a.Memory[0]<<endl;
    cout<<"0x04: "<<a.Memory[1]<<endl;
    cout<<"0x08: "<<a.Memory[2]<<endl;
    cout<<"0x0C: "<<a.Memory[3]<<endl;
    cout<<"0x10: "<<a.Memory[4]<<endl;
    cout<<endl;

    // IF ID
    cout<<"IF/ID :"<<endl;
    cout<<"PC\t\t"<<a.pc<<endl;
    cout<<"Instruction\t";
    for(int i=0;i<instructionLength;i++)
        cout<<a.instruction[i];
    cout<<endl;
    cout<<endl;

    // ID EX
    cout<<"ID/EX :"<<endl;
    cout<<"ReadData1\t"<<a.ReadData1<<endl;
    cout<<"ReadData2\t"<<a.ReadData2<<endl;
    cout<<"sign_ext\t"<<a.sign_ext<<endl;
    cout<<"Rs\t\t"<<a.Rs<<endl;
    cout<<"Rt\t\t"<<a.Rt<<endl;
    cout<<"Rd\t\t"<<a.Rd<<endl;
    cout<<"Control signals\t";
    for(int i=0;i<9;i++) cout<<a.ControlSignals1[i];
    cout<<endl;
    cout<<endl;

    // EX MEM
    cout<<"EX/MEM :"<<endl;
    cout<<"ALUout\t\t"<<a.ALUout1<<endl;
    cout<<"WriteData\t"<<a.WriteData<<endl;
    cout<<"Rt/Rd\t\t"<<a.RtOrRd1<<endl;
    cout<<"Control signals\t";
    for(int i=0;i<5;i++) cout<<a.ControlSignals2[i];
    cout<<endl;
    cout<<endl;

    // MEM WB
    cout<<"MEM/WB :"<<endl;
    cout<<"ReadData\t"<<a.ReadData3<<endl;
    cout<<"ALUout\t\t"<<a.ALUout2<<endl;
    cout<<"Rt/Rd\t\t"<<a.RtOrRd2<<endl;
    cout<<"Control signals\t";
    for(int i=0;i<2;i++) cout<<a.ControlSignals3[i];
    cout<<endl;
    cout<<endl;

    cout<<"============================================";

}

int main()
{
    ifstream ifs;
    ifs.open("SampleInput.txt", ios::in);

    /*string forTest;*/
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

    /*ifs>>forTest;
    cout<<forTest;*/




    pipe pipeline=pipe();

    for(int q=0;q<3;q++)
    {
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

        cout<<"CC: "<<q+1<<"\n"<<endl;
        pipeline.nextCC(input);
        printMenu(pipeline);
    }



    ifs.close();
}
