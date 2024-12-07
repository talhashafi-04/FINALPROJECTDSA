#include "AVL.h"
#include "Repository.h"

int main()
{
    cout << "ENTER THE PATH OF REPOSITORY : ";
    String path;
    cin >> path;
    cout << "ENTER TREE TYPE (AVL,BTREE,RBTREE) : ";
    String tree;
    cin >> tree;
    cout << "USE SHA-256 hash function ? (1/0) : ";
    bool useSHA256;
    cin >> useSHA256;

    Repository repo(path, tree, useSHA256);
    bool init = false;

    while (true)
    {
        String input;
        cin >> input;

        if (input == "exit")
        {
            break;
        }

        if (input == "git init")
        {
            cout << "Enter dataset FileName : ";
            String datasetPath;
            cin >> datasetPath;
            repo.initializeRepository(datasetPath);
            init = true;
            cout << "Initialized Successfully";
        }
        else if (input == "git commit")
        {
            cout << "Committing changes...\n";
            // Commit logic goes here
        }
        else if (input == "add data")
        {
            String name, roll,
            cout << "Enter new row space";
            
        }
        else if (input.startsWith("UPDATE"))
        {
            // Handle UPDATE commands
            String temp(100, 0);
            cin.getline(temp.data, 100); // Read the rest of the SQL command
            String command = temp.data;

            if (command.contains("BETWEEN"))
            {
                // Example: UPDATE Students SET Status = 'Promoted' WHERE RollNumber BETWEEN '21i-0066' AND '21i-0226';
                String tableName, columnName, value;
                String rangeStart, rangeEnd;
                sscanf_s(command.c_str(), "%s SET %[^ ] = '%[^']' WHERE %[^ ] BETWEEN '%[^']' AND '%[^']'",
                    tableName.data, columnName.data, value.data, columnName.data, rangeStart.data, rangeEnd.data);

                cout << "Updating " << columnName << " to " << value << " in " << tableName << " for range " << rangeStart << " to " << rangeEnd << '\n';

                // Call the tree's update function
                // e.g., repo.updateRange(tableName, columnName, value, rangeStart, rangeEnd);
            }
        }
        else if (input.startsWith("DELETE"))
        {
     
            String temp(100, 0);
            cin.getline(temp.data, 100); 
            String command = temp.data;

            if (command.contains("BETWEEN"))
            {
                // Example: DELETE FROM Students WHERE RollNumber BETWEEN '21i-0066' AND '21i-0226';
                String tableName, columnName, rangeStart, rangeEnd;
                sscanf_s(command.c_str(), "FROM %s WHERE %[^ ] BETWEEN '%[^']' AND '%[^']'",
                    tableName.data, columnName.data, rangeStart.data, rangeEnd.data);

                cout << "Deleting records from " << tableName << " where " << columnName << " is between " << rangeStart << " and " << rangeEnd << '\n';

                // Call the tree's delete range function
                // e.g., repo.deleteRange(tableName, columnName, rangeStart, rangeEnd);
            }
            else if (command.contains("LIMIT"))
            {
                // Example: DELETE FROM Students WHERE Name = 'Owais' LIMIT 1;
                String tableName, columnName, value;
                sscanf_s(command.c_str(), "FROM %s WHERE %[^ ] = '%[^']' LIMIT 1",
                    tableName.data, columnName.data, value.data);

                cout << "Deleting one record from " << tableName << " where " << columnName << " = " << value << '\n';

                // Call the tree's delete single record function
                // e.g., repo.deleteSingle(tableName, columnName, value);
            }
        }
        else
        {
            cout << "Unrecognized command: " << input << '\n';
        }
    }

    return 0;
}
