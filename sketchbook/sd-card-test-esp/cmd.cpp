#include "cmd.h"

static command_t commands[] = 
{
    {
        .name = String("ls"),
        .handle = listSd,
    },
    {
        .name = String("cat"),
        .handle = cat,
    },
    {
        .name = String("rm"),
        .handle = rm,
    },
    {
        .name = String("mkdir"),
        .handle = mkdir,
    },
    {
        .name = String("touch"),
        .handle = touch,
    },
    {
        .name = String("echo"),
        .handle = echo,
    },
    {
        .name = String("sta"),
        .handle = setSTA,
    },
    {
        .name = String("restart"),
        .handle = restart,
    },
    {
        .name = String("storeSettings"),
        .handle = storeSettings,
    },
    {
        .name = String("commands"),
        .handle = printCommands,
    },
    {
        .name = String("heap"),
        .handle = printHeap,
    },
    {
        .name = String("calc"),
        .handle = calc,
    }
};

static int numCommands = sizeof(commands) / sizeof(command_t);

void setupInterpreter()
{
    Serial.print("\n>> ");
}

int getNumCommands()
{
    return numCommands;
}

void setNumCommands(int amount)
{
    numCommands = amount;
}

void printArguments(int argc, String* argv)
{
    Serial.println();
    Serial.print(String("argc: ") + argc + " arguments: ");
    for(int i = 0; i < argc; i++)
    {
        Serial.print(argv[i]);
        Serial.print(' ');
    }
    Serial.println();
}

void initInterpreter()
{
    Serial.print(">> ");
}

// returns input that is newline terminated.
// returns string with 0 len if nothing found yet.
// or not done yet receiving.
String received = "";
String getInput()
{
    // check if anything new available.
    if(Serial.available())
    {
        // read in that data.
        char data = Serial.read();
        if(data == '\n' || data == '\r')
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
    static char* str_input = NULL;
    const char delim[] = " \n";
    char *svptr = NULL;
    if(input.length())
    {
        // create a copy to be used with strtok_r. free before if: it is used before.
        if(str_input != NULL)
        {
            free(str_input);
        }
        size_t str_input_len = (sizeof(char) * input.length()) + 1;
        str_input = (char *)malloc(str_input_len);
        if(str_input == NULL)
        {
            Serial.println("Input Copy Error.");
            return;
        }
        memset(str_input, '\0', input.length());
        strcpy(str_input, input.c_str());
        // check for leading whitespace.
        if(isspace(str_input[0]))
        {
            Serial.println("Unexpected White space.");
            return;
        }
        // trim trailing whitespace.
        int len = strlen(str_input);
        while(isspace(str_input[len - 1]))
        {
            str_input[len - 1] = '\0';
            len = strlen(str_input);
            delay(0);
        }

        // strtok merges adjecent delimeters (white space in this case)
        // that is good for us it means that it doesn't matter
        // how many white space we add between arguments.
        
        // first token is the actuall command.
        char* command = strtok_r(str_input, delim, &svptr);
        
        // max allowed length of argv, thus max allowed arguments.
        static int argc;
        static const uint8_t max_args = 20;
        String argv[max_args];

        // parse out all arguments for aslong as there are arguments.
        argc = 0;
        char* argument = strtok_r(NULL, delim, &svptr);
        while(argument != NULL)
        {
            // put the arguments in order in argv
            argv[argc] = String(argument);
            argument = strtok_r(NULL, delim, &svptr);
            argc++;
            // keep track of where to put argument in argv.
            // and check if we are not exeeding max arguments.
            // also is length of argv thus argc.
            if(argc >= max_args)
            {
                return;
            }
        }
        // Serial.println("argc: " + String(argc));

        // check for command in commands list. and call it's handler.
        for(int i = 0; i < numCommands;i++)
        {
            if(String(command) == commands[i].name)
            {
                // call that command handle
                (commands[i].handle)(argc, argv);
                // print this to show the interactive interpreter is back
                // to do our bidding!
                Serial.print("\n>> ");
                return;
            }
        }

        // if we reach here it's unkown input/command.
        Serial.println("unknown input: '" + input + "'");
        Serial.print(">> ");
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

void printHeap(int argc, String* argv)
{
    Serial.printf("heap: %d \n", ESP.getFreeHeap());
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
            Serial.print('\t');
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

void writeFile(int argc, String* argv)
{

}

void print3d(int argc, String* argv)
{

}

void mv(int argc, String* argv)
{

}

void echo(int argc, String* argv)
{
    for(int i = 0; i < argc; i++)
    {
        Serial.print(argv[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void calc(int argc, String* argv)
{

}

void printCommands(int argc, String* argv)
{
    Serial.println("Commands Available: ");
    for(int i = 0; i < numCommands; i++)
    {
        Serial.printf("--> %s\n", (const char *)commands[i].name.c_str());
    }
}

void touch(int argc, String* argv)
{
    File file;
    for(int i = 0; i < argc; i++)
    {
        if(argv[i].endsWith("/"))
        {
            Serial.printf("touch: setting times of ‘%s’: No such file or directory\n", argv[i].c_str());
            break;
        }
        else
        {
            file = SD.open(argv[i], FILE_WRITE);
            file.close();
        }
    }
}

void mkdir(int argc, String* argv)
{
    for(int i = 0; i < argc; i++)
    {
        SD.mkdir((char *)argv[i].c_str());
    }
}

void rm(int argc, String* argv)
{
    for(int i = 0; i < argc; i++)
    {
        String object = argv[i];
        if(SD.exists((char *)object.c_str()))
        {
            if(object.endsWith("/"))
            {
                SD.rmdir((char *)object.c_str());
            }
            else
            {
                SD.remove((char *)object.c_str());
            }
        }
        else
        {
            Serial.printf("rm: cannot remove %s: No such file or Directory", object.c_str());
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
            break;
        }
        file = SD.open(argv[i]);
        if(file.isDirectory())
        {
            Serial.println(String("cat: ") + argv[i] + ": " + "Is a Directory.");
            break;
        }
        while(file.available())
        {
            Serial.write((char)file.read());
        }
        // close file after reading.
        file.close();
    }
}

extern void setup();
extern void storeSettings();
void storeSettings(int, String*)
{
    // store settings.
    storeSettings();
    // restart network services
    setup();
}

// we need acces to the extern function setupWifi.
extern void setup();
extern void storeSettings();
extern String ssid;
extern String pass;
extern String board_name;
void setSTA(int argc, String* argv)
{
    if(argc >= 2 && argc < 4)
    {
        ssid = argv[0];
        pass = argv[1];
        // option boardname is set.
        if(argc == 3)
        {
            board_name = argv[2];
        }
        // store settings.
        storeSettings();
        // restart network services
        setup();
    }
    else
    {
        Serial.println("usage: sta ssid pass <boardname>");
    }
}

void restart(int argc, String* argv)
{
    int wait = 0;
    if(argc)
    {
        wait = argv[0].toInt();
    }
    delay(wait);
    ESP.restart();
}
