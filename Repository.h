#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "String.h"
#include "Vector.h"
#include "MerkelTree.h"
#include <filesystem>
#include <stdexcept>

#include "AVL.h"
using namespace std;
using namespace filesystem;

class Repository
{
private:
    String repo;
    String currentBranch;
    Vector<String> branches;
    String treeType;
    bool useSHA256;
    String currentTree;

    void saveBranches()
    {
        ofstream outFile((repo + "/branches.txt").c_str());
        if (!outFile)
        {
            cout << "Unable to save branches";
            exit(0);
        }
        for (int i = 0; i < branches.size(); i++)
        {
            outFile << branches[i] << '\n';
        }
        outFile.close();
    }

    void loadBranches()
    {
        ifstream inFile((repo + "/branches.txt").c_str());
        if (!inFile)
            return;

        branches.clear();
        String branchName(200, 0);
        while (inFile.getline(branchName.data, 200))
        {
            branches.push_back(branchName.data);
        }
        inFile.close();
    }

    void saveCurrentBranch()
    {
        ofstream branchFile((repo + "/current_branch.txt").c_str());
        if (!branchFile)
        {
            cout << "Unable to save the current branch";
            exit(0);
        }
        branchFile << currentBranch;
        branchFile.close();
    }

    void loadCurrentBranch()
    {
        ifstream branchFile((repo + "/current_branch.txt").c_str());
        if (branchFile)
        {
            String temp(100, 0);
            branchFile.getline(temp.data, 100);
            currentBranch = temp.data;
            branchFile.close();
        }
        else
        {
            currentBranch = "main";
        }
    }

    void initializeCurrentTree()
    {
        currentTree = repo + "/branches/" + currentBranch;
        if (!exists(currentTree.c_str()))
        {
            create_directories(currentTree.c_str());
        }
    }

    void saveSettings()
    {
        ofstream settingsFile((repo + "/settings.txt").c_str());
        if (!settingsFile)
        {
            cout << "Unable to save settings";
            exit(0);
        }
        settingsFile << treeType << '\n';
        settingsFile << useSHA256 << '\n';
        settingsFile.close();
    }

    void loadSettings()
    {
        ifstream settingsFile((repo + "/settings.txt").c_str());
        if (settingsFile)
        {
            String temp(100, 0);
            settingsFile.getline(temp.data, 100);
            treeType = temp.data;

            settingsFile.getline(temp.data, 100);
            useSHA256 = (temp == "1");
            settingsFile.close();
        }
        else
        {
            treeType = "AVL";
            useSHA256 = true;
        }
    }

public:
    Repository(const String directory = "", String tree = "AVL", bool useSHA256 = true)
        : repo(directory), treeType(tree), useSHA256(useSHA256)
    {
        if (!exists(directory.c_str()))
        {
            create_directories((repo).c_str());
            saveSettings();
        }
        else
        {
            loadSettings();
            loadBranches();
            loadCurrentBranch();
            initializeCurrentTree();
        }
    }

    void displayBranches()
    {
        loadBranches();
        for (int i = 0; i < branches.size(); i++)
        {
            cout << "BRANCH " << i+1 << " : " << branches[i] << endl;
        }
    }

    
    void deleteBranch(const String& branchName)
    {
       
        String path = repo + "/branches/" + branchName;

        if (!exists(path.data))
        {
            cout << "Branch does not exist.\n";
            return;
        }

        if (branchName == currentBranch) 
        {
            cout << "Cannot delete the current branch\n";
            return;
        }

        if (branchName == "main")
        {
            cout << "Cannot delete the main branch\n";
            return;
        }

        if (remove_all(path.data)) 
        {
            cout << branchName << " deleted successfully\n";

            for (int i = 0; i < branches.size(); i++)
            {
                if (branches[i] == branchName)
                {
                    branches.erase(i);
                    break;
                }
            }
          
            saveBranches();
        }
        else {
            cout << "Failed to delete the branch.\n";
        }
    }
    void initializeRepository(String csvPath)
    {
        if (!exists(repo.c_str()))
        {
            create_directories((repo + "/branches").c_str());
        }

        String master = repo + "/branches/main";
        if (!exists(master.c_str()))
        {
            create_directories(master.c_str());
            create_directories((master + "/tree").c_str()); // Create tree directory
            ofstream rootFile((master + "/tree/root.txt").c_str()); // Create root.txt
            rootFile << "null"; // Initially, no root node
            rootFile.close();

            ofstream logs((master + "/main_log.txt").c_str());
            logs.close();
        }

        currentBranch = "main";
        branches.push_back("main");
        saveBranches();
        saveCurrentBranch();
        initializeCurrentTree();

        load(csvPath);

    }

    
    void createBranch(const String& branchName)
    {
        String baseBranch = repo + "/branches/" + currentBranch;
        String newBranch = repo + "/branches/" + branchName;

        if (exists(newBranch.data))
        {
            cout << "Branch already exists";
            return;
        }

        create_directories(newBranch.data);
        create_directories((newBranch + "/tree").c_str()); // Create tree directory
        ofstream rootFile((newBranch + "/tree/root.txt").c_str()); // Create root.txt
        rootFile << "null"; // Initially, no root node
        rootFile.close();

        // Copy log file
        copy((baseBranch + "/" + currentBranch + "_log.txt").c_str(),
            (newBranch + "/" + branchName + "_log.txt").c_str());

        branches.push_back(branchName);
        saveBranches();
    }


    void checkout(const String& branchName)
    {
        bool branchExists = false;
        for (int i = 0; i < branches.size(); i++)
        {
            if (branches[i] == branchName)
            {
                branchExists = true;
                break;
            }
        }

        if (!branchExists)
        {
            cout << "Branch does not exist";
            return;
        }

        currentBranch = branchName;
        saveCurrentBranch();
        initializeCurrentTree();
    }

    String getCurrentBranch() const
    {
        return currentBranch;
    }

    Vector<String> listBranches() const
    {
        return branches;
    }

    void status() const
    {
        cout << "Repository Directory: " << repo << '\n';
        cout << "Current Branch: " << currentBranch << '\n';
        cout << "Tree Type: " << treeType << '\n';
        cout << "Using SHA256: " << (useSHA256 ? "Yes" : "No") << '\n';
        cout << "Branches:\n";
        for (int i = 0; i < branches.size(); i++)
        {
            cout << "- " << branches[i] << '\n';
        }
    }
    void displayCurrentBranch()
    {
        loadCurrentBranch();
        cout << currentBranch;
    }

    void insert(const String& key, const String& csvRow, int csvRowNumber)
    {
        String treeFile = currentTree + "/" + currentBranch + "_tree.dat";
        ofstream outFile(treeFile.c_str(), ios::app);
        if (!outFile)
        {
            cout << "Unable to open tree file";
            return;
        }
        outFile << key << ',' << csvRow << ',' << csvRowNumber << '\n';
        outFile.close();
    }

    void commit(const String& commitMessage)
    {
        //check if commit exist, is this first commit?
        
        
        //do this

        //make merkel tree of last commit and current tree , wheather there are changes or not?

        String root;
        String path = repo + "/branches/" + currentBranch + "/tree/";
        {
            AVL avl(path+"root.txt");
            root = avl.rootFile;
        }
        if (treeType == "avl" || treeType == "AVL")
        {
            String path = repo + "/branches/" + currentBranch + "/tree/";
            AVL avl(path + "root.txt");
            Vector<String> blocks;
            avl.getBlocks(blocks);
            MerkleTree tree(blocks,useSHA256);
            String rootHash = tree.getRootHash();
        }
        
       


    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

   
    void visualizeTree(const String & branchName)
    {
        if (treeType == "AVL" || treeType == "avl" )
        {
            String path = repo + "/branches/" + branchName + "/tree/root.txt";
            AVL avl(path);
            avl.printTree();
        }
    }
    void load(const String& csvPath)
    {

        String treePath = repo + "/branches/" + currentBranch + "/tree";
        for (const auto& entry : directory_iterator(treePath.c_str()))
        {
            remove_all(entry.path());
        }

        ofstream rootFile((treePath + "/root.txt").c_str());
        rootFile.close();

        ifstream csvFile(csvPath.c_str());
        if (!csvFile)
        {
            cout << "Unable to open CSV file";
            return;
        }

        String temp(100, 0);
        csvFile.getline(temp.data, 100);
        String line = temp.data;

        Vector<String> columns;
        temp = String(100, 0);
        int index = 0;
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == ',')
            {
                columns.push_back(temp.data);
                index = 0;
                temp = String(100, 0);
                continue;
            }

            temp[index++] = line[i];
        }
        columns.push_back(temp.data);

        cout << "\nSelect a column \n";
        for (int i = 0; i < columns.size(); i++)
        {
            cout << i << ": " << columns[i] << '\n';
        }

        int selectedCol;
        cin >> selectedCol;

        if (selectedCol < 0 || selectedCol >= columns.size())
        {
            cout << "Invalid column selected";
            return;
        }

        cout << "You selected column: " << columns[selectedCol] << '\n';

        if (treeType == "AVL" || treeType == "avl")
        {
            AVL avl((treePath +  "/root.txt").c_str());
            temp = String(100, 0);
            int csvRowNum = 0;
            while (csvFile.getline(temp.data, 100))
            {
                csvRowNum++;
                String line = temp.data;
                temp = String(100, 0);


                int index = 0;
                int count = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (count == selectedCol)
                    {
                        if (line[i] == ',')
                        {
                            temp[index] = 0;
                            break;
                        }
                        temp[index++] = line[i];
                    }
                    if (line[i] == ',')
                    {
                        count++;
                    }
                }

                avl.insert(line, temp.data, generateUniquePath(treePath +"/file"));
            }

        }

    }
    void update(String key, String newRow)
    {
        if (treeType == "avl" || treeType == "AVL")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            AVL avl(Path);
            avl.update(key, newRow);

        }
    }
    void add(String key, String newRow)
    {
        if (treeType == "avl" || treeType == "AVL")
        {
            String Path = repo + "/branches/" + currentBranch + "/tree/root.txt";
            AVL avl(Path);
            avl.insert( newRow,key, generateUniquePath(repo + "/branches/" + currentBranch + "/tree/file"));

        }
    }

    String generateUniquePath(const String& path)
    {
        srand(time(0));
        while (true)
        {
            String newP = path + String::toString(rand() % 100000);
            ifstream file((newP+".node").data);
            if (!file)
            {
                return newP;
            }
            file.close();
        }
    }
};

