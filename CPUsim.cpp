#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>

struct MEMORY
{
    int8_t memory[256] = {};    // std::cout treats uint8_t as a ASCII code , format to int beforehand

    void DEBUG_LIST_MEMORY()
    {
        for(int i=0 ; i<256 ; i++)
        {
            std::cout<<"MEM ADDRESS : " << i << "  |  VALUE : " << (int)memory[i] << std::endl;
        }
    }
    void DEBUG_CHECK_MEMORY_VALUE(unsigned int MEMORY_ADDRESS)
    {
        if(MEMORY_ADDRESS >= 256){std::cout << "[DEBUG_CHECK_MEMORY_VALUE] error : MEMORY_ADDRESS is out of range (0-256) !" << std::endl;return;}
        std::cout<<"MEMORY ADDRESS : " << MEMORY_ADDRESS << "  |  VALUE : " << (int)memory[MEMORY_ADDRESS] << std::endl;
    }
};

struct CPU
{
    int8_t A = 0b00000000;
    int8_t B = 0b00000000;

    unsigned int PC = 0;

    void LOAD(const char REG_NAME,unsigned int MEM_ADDRESS,MEMORY &MEM)
    {
        if(MEM_ADDRESS >= 256){std::cout<<"[LOAD] error : MEM_ADDRESS is not in MEM_SIZE range (0-256) !"<<std::endl;return;}
        switch (REG_NAME)
        {
            case 'A':
            A = MEM.memory[MEM_ADDRESS];
            break;
            case 'B':
            B = MEM.memory[MEM_ADDRESS];
            break;
            default:
            std::cout<<"[LOAD] error : REG_NAME not found ! , value could not be loaded"<<std::endl;

        }
    }

    void DEBUG_CHECK_REG_VAL(const char REG_NAME)
    {
        switch (REG_NAME)
        {
            case 'A':
            std::cout << "REG A : " << (int)A << std::endl;
            break;
            case 'B':
            std::cout << "REG B : " << (int)B << std::endl;
            break;
            default:
            std::cout << "[DEBUG_CHECK_REG_VAL] error : REG_NAME not found ! , value could not checked"<<std::endl;
        }
    }

    void STORE(const char REG_NAME,MEMORY &MEM,unsigned int MEMORY_ADDRESS)
    {
        switch (REG_NAME)
        {
        case 'A':
        MEM.memory[MEMORY_ADDRESS] = A;    
        break;
        case 'B':
        MEM.memory[MEMORY_ADDRESS] = B;    
        break;
        default:
        std::cout<<"[STORE] error : REG_NAME not found ! , value could not be stored"<<std::endl;
        break;
        }
    }

    //                       ALU                      //

    void ADD(const char FRONT_REG,const char BACK_REG,const char STORE_REG)
    {
        if(FRONT_REG=='A' && BACK_REG=='B')
        {
            if(STORE_REG=='A'){A = (int)A + (int)B;}
            else if(STORE_REG=='B'){B = (int)A + (int)B;}
            else {std::cout<<"[ADD] error : STORE_REG not found ! , REG A,B could not be added"<<std::endl;}
        }else if (FRONT_REG=='A' && BACK_REG=='A')
        {
            if(STORE_REG=='A'){A = (int)A + (int)A;}
            else if(STORE_REG=='B'){B = (int)A + (int)A;}
            else {std::cout<<"[ADD] error : STORE_REG not found ! , REG A,A could not be added"<<std::endl;}
        }else if (FRONT_REG=='B' && BACK_REG=='B')
        {
            if(STORE_REG=='A'){A = (int)B + (int)B;}
            else if(STORE_REG=='B'){B = (int)B + (int)B;}
            else {std::cout<<"[ADD] error : STORE_REG not found ! , REG B,B could not be added"<<std::endl;}
        }else
        {
            std::cout<<"[ADD] error : FRONT_REG , BACK_REG COMBINATION not found ! , please re-check reg names"<<std::endl;}
    }

    void SUB(const char FRONT_REG,const char BACK_REG,const char STORE_REG)
    {
        if(FRONT_REG=='A' && BACK_REG=='B')
        {
            if(STORE_REG=='A'){A = (int)A - (int)B;}
            else if(STORE_REG=='B'){B = (int)A - (int)B;}
            else {std::cout<<"[SUB] error : STORE_REG not found ! , REG A,B could not be subtracted"<<std::endl;}
        }
        else if(FRONT_REG=='B' && BACK_REG=='A')
        {
            if(STORE_REG=='A'){A = (int)B - (int)A;}
            else if(STORE_REG=='B'){B = (int)B - (int)A;}
            else {std::cout<<"[SUB] error : STORE_REG not found ! , REG B,A could not be subtracted"<<std::endl;}
        }
        else if(FRONT_REG=='A' && BACK_REG=='A')
        {
            if(STORE_REG=='A'){A = 0;}
            else if(STORE_REG=='B'){B = 0;}
            else {std::cout<<"[SUB] error : STORE_REG not found ! , REG A,A could not be subtracted"<<std::endl;}
        }
        else if (FRONT_REG=='B' && BACK_REG=='B')
        {
            if(STORE_REG=='A'){A = 0;}
            else if(STORE_REG=='B'){B = 0;}
            else {std::cout<<"[SUB] error : STORE_REG not found ! , REG B,B could not be subtracted"<<std::endl;}
        }
        else
        {
            std::cout<<"[SUB] error : FRONT_REG , BACK_REG COMBINATION not found ! , please re-check reg names"<<std::endl;}
    }

    //                       ALU                      //

    void MOV(const char DESTINATION_REG,const char SOURCE_REG)
    {
        switch (DESTINATION_REG)
        {
            case 'A':
            switch (SOURCE_REG)
                {
                    case 'A':  
                    break;
                    case 'B':
                    A = B;
                    break;
                    default:
                    std::cout << "[MOV] error : SOURCE_REG not found ! , could not copy reg " << std::endl;
                    break;
                }
            break;
            case 'B':
            switch (SOURCE_REG)
                {
                    case 'B':  
                    break;
                    case 'A':
                    B = A;
                    break;
                    default:
                    std::cout << "[MOV] error : SOURCE_REG not found ! , could not copy reg " << std::endl;
                    break;
                }
            break;
            default:
                std::cout << "[MOV] error : DESTINATION_REG not found ! , could not copy reg " << std::endl;
            break;
        }    
    }

    void CMP(const char FRONT_REG, const char BACK_REG)
    {
        switch (FRONT_REG)
        {
            case 'A':
                switch (BACK_REG)
                {
                  case 'A':
                  std::cout << "[CMP] message : A reg is equal to A reg" << std::endl;
                  break;
                  case 'B':
                    if(A > B){std::cout << "[CMP] message : A reg is bigger than B reg" << std::endl;}
                    else if (A < B){std::cout << "[CMP] message : A reg is smaller than B reg" << std::endl;}
                    else {std::cout << "[CMP] message : A reg is equal to B reg" << std::endl;}
                  break;
                  default:
                  std::cout << "[CMP] error : BACK_REG was not found !" << std::endl;
                  break;
                }
            break;
            case 'B':
                switch (BACK_REG)
                {
                  case 'B':
                  std::cout << "[CMP] message : B reg is equal to B reg" << std::endl;
                  break;
                  case 'A':
                    if(B > A){std::cout << "[CMP] message : B reg is bigger than A reg" << std::endl;}
                    else if (B < A){std::cout << "[CMP] message : B reg is smaller than A reg" << std::endl;}
                    else {std::cout << "[CMP] message : B reg is equal to A reg" << std::endl;}
                  break;
                  default:
                  std::cout << "[CMP] error : BACK_REG was not found !" << std::endl;
                  break;
                }
            break;
            default:
            std::cout << "[CMP] error : FRONT_REG was not found !" << std::endl;
            break;
        }
    }

    void LC(const char REG_NAME,int8_t VALUE)
    {
        switch (REG_NAME)
        {
        case 'A':
        A = VALUE;    
        break;
        case 'B':
        B = VALUE;
        break;
        default:
        std::cout<<"[LC] error : REG_NAME [" << REG_NAME << "] not found !"<<std::endl;
        break;
        }
    }

    bool CHECK_REG(const char REG_NAME){return (REG_NAME=='A' || REG_NAME=='B');}
};

struct EXE
{
    void fetch(const std::string command , CPU &cpu , MEMORY &mem)
    {
        // LD [REG] #$ [INT VALUE] //
        if(command.find("LD") != std::string::npos)
        {
            const char REG = command[2];
            // index 6 with length 2 [6->8] retrieves the INT VALUE //
            if(command[4]=='#')
            {
                int8_t VAL = std::stoi(command.substr(6,2));
                cpu.LC(REG,VAL);
                if(!cpu.CHECK_REG(REG)){return;}
                std::cout<<"[FETCH] message : loaded the value "<<(int)VAL<<" into the REG " << REG << " (from #) " << std::endl;
            }else
            {
                int8_t VAL = std::stoi(command.substr(5,1));
                cpu.LOAD(REG,(int)VAL,mem);
                if(!cpu.CHECK_REG(REG)){return;}
                std::cout<<"[FETCH] message : loaded the value "<<(int)mem.memory[(int)VAL]<<" into the REG " << REG << " (from memory) " << std::endl;
            }
        }
    }

    void EXECUTE_PROGRAM(const std::string PROGRAM_NAME , CPU &cpu , MEMORY &mem)
    {
        std::fstream file(PROGRAM_NAME);
        if(!file.is_open()){std::cout<<"[EXECUTE PROGRAM] error : file could not be opened ! "<<std::endl;return;}
        std::cout<<"[EXECUTE_PROGRAM] message : opened the "<<PROGRAM_NAME<< " file !"<<std::endl; 
        std::string line;
        while(std::getline(file,line))
        {
            fetch(line,cpu,mem);
        }
    }
};


int main()
{
    CPU cpu;
    MEMORY mem;
    EXE exe;
    exe.EXECUTE_PROGRAM("program.txt",cpu,mem);
    return 0;
}

// todo add rest of commands to the fetch function // 