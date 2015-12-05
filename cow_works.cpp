#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>


using namespace std;

#define tag 0

void farmer(const int& size)
{
		srand( time(0) );
		int a = rand()%10 + 2;
		int* seno = new int [a];
        
        double milk = 0;
        for (int i = 0; i < a; i++) seno[i]= rand()%11+2;
       seno[a]=0;
        //cout<<seno[0];
        int cows = size - 1;
        double buf[2];
        int k = 0;
        int count = 0; //how many cows didn't receive seno
        while (true)
        { // cows alive
                MPI_Status status;
                MPI_Recv(buf, 2, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status );
                //printf("%s %d\n", "I am farmer. recived signal from cow № ", status.MPI_SOURCE );
                if (buf[0] == 0) //cow is hungry
                {
                        printf("%s %d %s\n", "I am farmer. Recived signal from cow № ", status.MPI_SOURCE, " Cow is hungry. " );
                        buf[0] = seno[k];
                        //seno[k] = 0;
                        MPI_Send(buf,2,MPI_DOUBLE, status.MPI_SOURCE, tag, MPI_COMM_WORLD);
                        printf("%s %d\n", "I am farmer. Sent seno to cow № ", status.MPI_SOURCE);
                        if (k<a) k++;
                }
                if (buf[0] == 1) //cow is sending milk
                {
                        milk = milk+ buf[1];
                        printf("%s %d %s %.4f\n", "I am farmer. Recived signal from cow № ", status.MPI_SOURCE, " Cow sent milk: ", buf[1] );
                }
                if(buf[0] ==  2)
                {
                        printf("%s %d %s\n", "I am farmer. Cow №", status.MPI_SOURCE, " is out of seno. " );
                        count++;
                }
                if (count == size - 1) {
                        //cout<<"milk: "<<milk;
                        printf("%s %10.4f\n", "IIIIIIIIIIIIIIIIIIIIIIIIIII am farmer. All cows are out of seno. Milk: ", milk );
                        return;
                }
        }
        return;
}

void cow(const int& id)
{
        double buf[2]={0,0};
        while (true)
        {
                MPI_Send(buf, 2, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD ); //send that she is hungry
                printf("%s %d %s\n", "I am a cow № ", id, " I sent signal, that I am hungry. ");
                MPI_Status status;
				MPI_Recv(buf, 2, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status); //cow receives seno
                if (buf[0]==0)
                {
                        printf("%s %d %s\n", "I am a cow № ", id, " I didn't receive seno. " );
                        buf[0]=2; // cow didn't receive seno
                        MPI_Send(buf, 2, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD );
                        printf("%s %d %s\n", "I am a cow № ", id , " I sent signal, that I am out of seno. " );
                        //cout<<"cow is dead";
                        return;
                }
                printf("%s %d %s\n", "I am a cow № ", id, " I received seno. " );
				
                
                buf[1] = buf[0]*(((rand()%21)+30)/100.0); //how much milk she did (from 0.3 to 0.5)
				buf[0] = 1; //cow is not hungry, cow is eating
				
               
                MPI_Send(buf, 2, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD); //send milk
                printf("%s %d %s %10.7f\n", "I am a cow № ", id, " I sent milk: ", buf[1] );
                buf[0] =0; //cow is hungry again
                buf[1] =0;
        }
        return;
}

void MPI_Body()
{
        int rank, size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) farmer(size);
        else cow(rank);
}

int main(int argc, char* argv[]){
        MPI_Init(&argc, &argv);
        MPI_Body();
        MPI_Finalize();
        return 0;

}

