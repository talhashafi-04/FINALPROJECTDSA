//#include "AVL.h"
//#include "Repository.h"
//
//int main()
//{
//    cout << "ENTER THE PATH OF REPOSITORY : ";
//    String path;
//    cin >> path;
//    cout << "ENTER TREE TYPE (AVL,BTREE,RBTREE) : ";
//    String tree;
//    cin >> tree;
//    cout << "USE SHA-256 hash function ? (1/0) : ";
//    bool useSHA256;
//    cin >> useSHA256;
//
//    Repository repo(path, tree, useSHA256);
//    bool init = false;
//
//    String command;
//    while (true) {
//        cout << "> ";
//        cin >> command;
//
//
//        if (command == "commands")
//        {
//            cout << "init\nbranch\ncheckout\ncommit\nbranches\ndelete-branch\nmerge-tree\nvisualize-tree\n\log\current-branch\nload\nsave\nexit\n";
//            cout << "\nupdate\nadd,delete\n";
//        }
//        else if (command == "update")
//        {
//            cout << "Enter Key for updation : ";
//            String key;
//            cin >> key;
//            
//            String newRow;
//            cout << "Enter updated data(CSV ROW)";
//            cin >> newRow;
//
//            repo.update(key, newRow);
//        }
//        else if (command == "add")
//        {
//            String key;
//            cout << "Enter key : ";
//            cin >> key;
//            String row;
//            cout << "Enter row : ";
//            cin >> row;
//            repo.add(key,row);
//        }
//        else if (command == "del")
//        {
//            String key;
//            cout << "Enter key : ";
//            cin >> key;
//            
//           //repo.del(key, row);
//        }
//        else if (command == "init")
//        {
//            String temp;
//            cout << "Enter CSV File Path : ";
//            cin >> temp;
//            repo.initializeRepository(temp);
//            init = true;
//        }
//        else if (command == "branch")
//        {
//            String branchName;
//            cin >> branchName;
//            repo.createBranch(branchName);
//        }
//        else if (command == "checkout") {
//            String branchName;
//            cout << "Enter Branch Name : ";
//            cin >> branchName;
//            repo.checkout(branchName);
//        }
//        else if (command == "commit")
//        {
//            String message;
//            cout << "Enter commit message : ";
//            cin >> message;
//            repo.commit(message);
//        }
//        else if (command == "branches")
//        {
//            repo.displayBranches();
//        }
//        else if (command == "delete-branch") {
//            String branchName;
//            cin >> branchName;
//            repo.deleteBranch(branchName);
//        }
//        else if (command == "merge")
//        {
//            String source, target;
//            cout << "Source : ";
//            cin >> source;
//            cout << "Target : ";
//            cin >> target;
//           // merge(source, target);
//        }
//        else if (command == "visualize-tree")
//        {
//            String branchName;
//            cout << "Enter branch name : ";
//            cin >> branchName;
//            repo.visualizeTree(branchName);
//        }
//        else if (command == "log")
//        {
//            //displayLog();
//        }
//        else if (command == "current-branch")
//        {
//            repo.displayCurrentBranch();
//            cout << endl;
//        }
//        else if (command == "save")
//        {
//            //save();
//        }
//        else if (command == "load")
//        {
//            String fileName;
//            cin >> fileName;
//            repo.load(fileName);
//        }
//        else if (command == "exit")
//            break;
//        else if (command != "")
//            cout << "INVALID command.\n";
//
//    }
//
//        //if (input.startsWith("UPDATE"))
//        //{
//        //    // Handle UPDATE commands
//        //    String temp(100, 0);
//        //    cin.getline(temp.data, 100); // Read the rest of the SQL command
//        //    String command = temp.data;
//
//        //    if (command.contains("BETWEEN"))
//        //    {
//        //        // Example: UPDATE Students SET Status = 'Promoted' WHERE RollNumber BETWEEN '21i-0066' AND '21i-0226';
//        //        String tableName, columnName, value;
//        //        String rangeStart, rangeEnd;
//        //        sscanf_s(command.c_str(), "%s SET %[^ ] = '%[^']' WHERE %[^ ] BETWEEN '%[^']' AND '%[^']'",
//        //            tableName.data, columnName.data, value.data, columnName.data, rangeStart.data, rangeEnd.data);
//
//        //        cout << "Updating " << columnName << " to " << value << " in " << tableName << " for range " << rangeStart << " to " << rangeEnd << '\n';
//
//        //        // Call the tree's update function
//        //        // e.g., repo.updateRange(tableName, columnName, value, rangeStart, rangeEnd);
//        //    }
//        //}
//        //else if (input.startsWith("DELETE"))
//        //{
//     
//        //    String temp(100, 0);
//        //    cin.getline(temp.data, 100); 
//        //    String command = temp.data;
//
//        //    if (command.contains("BETWEEN"))
//        //    {
//        //        // Example: DELETE FROM Students WHERE RollNumber BETWEEN '21i-0066' AND '21i-0226';
//        //        String tableName, columnName, rangeStart, rangeEnd;
//        //        sscanf_s(command.c_str(), "FROM %s WHERE %[^ ] BETWEEN '%[^']' AND '%[^']'",
//        //            tableName.data, columnName.data, rangeStart.data, rangeEnd.data);
//
//        //        cout << "Deleting records from " << tableName << " where " << columnName << " is between " << rangeStart << " and " << rangeEnd << '\n';
//
//        //        // Call the tree's delete range function
//        //        // e.g., repo.deleteRange(tableName, columnName, rangeStart, rangeEnd);
//        //    }
//        //    else if (command.contains("LIMIT"))
//        //    {
//        //        // Example: DELETE FROM Students WHERE Name = 'Owais' LIMIT 1;
//        //        String tableName, columnName, value;
//        //        sscanf_s(command.c_str(), "FROM %s WHERE %[^ ] = '%[^']' LIMIT 1",
//        //            tableName.data, columnName.data, value.data);
//
//        //        cout << "Deleting one record from " << tableName << " where " << columnName << " = " << value << '\n';
//
//        //        // Call the tree's delete single record function
//        //        // e.g., repo.deleteSingle(tableName, columnName, value);
//        //    }
//        //}
//        //else
//        //{
//        //    cout << "Unrecognized command: " << input << '\n';
//        //}
//
//    return 0;
//}
// 