#include <iostream>
#include <string>
#include <fstream>

using namespace std; 

//struct for info from input.txt plus a boolean to indicate if job is complete
struct job
{
    int pid;
    int require;
    int length;
    bool is_done; 
};

//function serving as default constructors
void init(struct job* schedule) 
{
    for (int i = 0; i < 1001; i++)
    {
        schedule[i].pid = 0;
        schedule[i].require = 0;
        schedule[i].length = 0;
        schedule[i].is_done = false;
    }
}

//insert data from input.txt 
void insert(struct job* schedule, string read, int num) 
{
    string::size_type str1;
    string temp[3];
    char cstr[read.length()];
    int count = 0;

    //copy string into cstr and insert into the variables - will convert strings into integers for array
    read.copy(cstr, read.length()); 
    for (int i = 0; i < read.length(); i++) 
    {
      if (static_cast<int>(cstr[i]) == 44) //if comma, remove it
        {
            ++count;
            continue;
        }
      else if (static_cast<int>(cstr[i]) == 32) //if space, remove it
            continue;
        else
            temp[count] += cstr[i];
    }
    //convert the strings to ints and place in struct vars
    schedule[num].pid = stoi(temp[0], &str1); 
    schedule[num].require = stoi(temp[1], &str1);
    schedule[num].length = stoi(temp[2], &str1);
}

//read input.txt file and place in a buffer big enough to hold the data
int read_input(struct job* schedule) 
{
    int num = 0;
    string buffer;

    fstream jobs;
    jobs.open("job.txt", fstream::in);
    if (jobs.is_open()) 
        {
            while (jobs)
            {
                jobs >> buffer;
                insert(schedule, buffer, num);
                num++;
            }
            return 0;
        } 
        else
        {
            cout << "error:  file not found" << endl;
            return 1;
        }
        jobs.close();
}

//find the duration of all jobs
int calc_dur(struct job* schedule) 
{
    int total = 0;

    for (int i = 0; i < 1001; i++)
        total += schedule[i].length;
    return total;
}

//'round robin scheduler'
void round_robin(struct job* schedule) 
{
    int burst = 5;
    int curr_time = 0;
    int timer = 0;    
    int total = calc_dur(schedule);

    for (curr_time = 0; curr_time < total; curr_time++)
    {
        for (int i = 0; i < 1001; i++)
        {
	  //if the job is still in progress
	  if (schedule[i].require <= curr_time && schedule[i].is_done == false)
            {
                cout << "Job " << schedule[i].pid << " is scheduled for 5ms";

		//flag bool that job is_done
                if (schedule[i].length <= burst) 
                {
                    cout << " and is complete" << endl;
                    schedule[i].is_done = true;
                }
                else //subtract burst time
                {
                    schedule[i].length -= burst;
		    cout << endl;
                   
                }
            }
        }
    }
 }

int main() //driver
{
  //array to hold the 1000 jobs from input.txt
    job *schedule = new job[1001]; 

    //initialize defalut vars
    init(schedule); 

    //read job schedule text file - input.txt
    if (read_input(schedule)) 
        return -50;

    //create the round robin scheduling
    round_robin(schedule); 

    //makes sure to delete all of the allocated data after process runs
    delete[] schedule;
}
