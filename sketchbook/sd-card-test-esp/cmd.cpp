#include "cmd.h"

command_t commands[] = 
{
    {
        .name = String("list"),
        .handle = listSd,
    },
    {
        .name = String("cat"),
        .handle = cat,
    }
};

int numCommands = sizeof(commands) / sizeof(command_t);

int getNumCommands()
{
    return numCommands;
}

void setNumCommands(int amount)
{
    numCommands = amount;
}

// returns input that is newline terminated.
// returns string with 0 len if nothing found yet.
// or not done yet receiving.
String received;
String getInput()
{
    // check if anything new available.
    if(Serial.available())
    {
        // read in that data.
        char data = Serial.read();
        if(data == '\n')
        {
            // if end of line (marked with newline) is found.
            // return a copy and clear our own buffer.
            String nreceived = received;
            received = "";
            return nreceived;
        }
        else
        {
            // else just keep appending that new data!
            received += data;
        }
    }
    return String("");
}


// this function interprets the data.
// and executes it.
void interpretInput(String input)
{
    static const char delim[] = " ";
    if(input.length())
    {
        // print command to prompt.
        Serial.println(">> " + input);
        
        // create a copy to be used with strtok.
        char* str_input = (char *)malloc(sizeof(char) * input.length());
        if(str_input == NULL)
        {
            Serial.println("Input Copy Error.");
            return;
        }
        strcpy(str_input, input.c_str());
        // check for leading whitespace.
        if(isspace(str_input[0]))
        {
            Serial.println("Syntax error");
            free(str_input);
            return;
        }
        // trim trailing whitespace.
        int len = strlen(str_input);
        while(isspace(str_input[len - 1]))
        {
            str_input[len - 1] = '\0';
            len = strlen(str_input);
        }

        // strtok makes inter command/argument whitespaces appear and thus
        // no need to check for that.
        
        // first token is the actuall command.
        String command = String(strtok(str_input, delim));
        // Serial.println("command: " + command + " len: " + command.length());
        
        // max allowed length of argv, thus max allowed arguments.
        static int argc;
        static const uint8_t max_args = 20;
        String argv[max_args];

        // parse out all arguments for aslong as there are arguments.
        char* argument = strtok(NULL, delim);
        argc = 0;
        while(argument != NULL)
        {
            // put the arguments in order in argv
            argv[argc] = String(argument);
            // Serial.println("Argument: " + argv[argc] + " Len: " + argv[argc].length());
            argument = strtok(NULL, delim);
            argc++;
            // keep track of where to put argument in argv.
            // and check if we are not exeeding max arguments.
            // also is length of argv thus argc.
            if(argc == max_args)
            {
                free(str_input);
                return;
            }
        }
        // Serial.println("argc: " + String(argc));

        // check for command in commands list. and call it's handler.
        for(int i = 0; i < numCommands;i++)
        {
            if(command == commands[i].name)
            {
                // call that command handle
                (commands[i].handle)(argc, argv);
                free(str_input);
                return;
            }
        }

        // if we reach here it's unkown input/command.
        Serial.println("unknown input: '" + input + "'");
        free(str_input);
    }
}

void printFile(File file)
{
    Serial.print("\t");
    Serial.print(file.name());
    Serial.print("\t\t");
    unsigned int file_size = file.size();
    String size_order = "";
    if (file_size > (1024 * 1024 * 1024))
    {
        file_size /= (1024 * 1024 * 1024);
        size_order = "GB";
    }
    else if(file_size > (1024 * 1024))
    {
        file_size /= (1024 * 1024);
        size_order = "MB";
    }
    else if(file_size > 1024)
    {
        file_size /= 1024;
        size_order = "KB";
    }
    Serial.print(file_size, DEC);
    Serial.println(size_order);

}

void printDirectory(File dir, int numTabs)
{
    while(true)
    {
        File entry = dir.openNextFile();
        if(!entry)
        {
            break;
        }
        for(uint8_t i = 0; i < numTabs;i++)
        {
            Serial.print('\t');
        }
        if(entry.isDirectory())
        {
            Serial.print(entry.name());
            Serial.println("/");
            printDirectory(entry, numTabs+1);
        }
        else
        {
            printFile(entry);
        }
        entry.close();
    }
}

void listSd(int argc, String* argv)
{
    // printArguments(argc, argv);
    File file;
    for(int i = 0; i < argc; i++)
    {
        // if file/directory exists list it
        // else print error message.
        // check for root first.
        if((argv[i] == String("/")) || (SD.exists((char *)argv[i].c_str())))
        {
            file = SD.open(argv[i]);
            Serial.println(argv[i] + ": ");
            // printDirectory closes the file.
            if(file.isDirectory()){
                file.rewindDirectory();
                printDirectory(file, 0);
            }
            else
            {
                printFile(file);
                file.close();
            }
        }
        else
        {
            Serial.print("list: ");
            Serial.print("cannot access ");
            Serial.print(argv[i]);
            Serial.println(": No such file or directory.");
        }
    }
}

void cat(int argc, String* argv)
{
    File file;
    for(int i = 0; i < argc; i++)
    {
        if(!SD.exists((char *)argv[i].c_str()))
        {
            Serial.print("cat: ");
            Serial.print(argv[i]);
            Serial.print(": No such file.");
        }
        else if(file.isDirectory())
        {
            Serial.println(String("cat: ") + argv[i] + ": " + "Is a Directory.");
        }
        else
        {
            file = SD.open(argv[i]);
            while(file.available())
            {
                Serial.write((char)file.read());
            }
            // close file after reading.
            file.close();
        }
    }
}
