#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//datatype that holds the phone number and name
struct contact
{
   string name;
   int phoneNumber;

   contact() { name = ""; phoneNumber = 0; };
   contact( string nm, int pn ) { name = nm; phoneNumber = pn; };
};

// Node class
class Node 
{
    contact key;
    Node* left;
    Node* right;
public:
    Node() { left=NULL; right=NULL; };
    void setKey(contact aKey) { key = aKey; };
    void setLeft(Node* aLeft) { left = aLeft; };
    void setRight(Node* aRight) { right = aRight; };
    contact Key() { return key; };
    int KeyPN() { return key.phoneNumber; };
    string KeyName() { return key.name; };
    Node* Left() { return left; };
    Node* Right() { return right; };
};

// Tree class
class Tree 
{
     Node* root;
public:
     Tree();
     ~Tree();
     Node* Root() { return root; };
     void addNode(contact key);
     void inOrder(Node* n);
     void preOrder(Node* n);
     void delete_node(Node* n, int leaf);
     void search_bst(Node* n, int leaf);
     Node* lowest(Node* n);
private:
     void addNode(contact key, Node* leaf);
     void freeNode(Node* leaf);
};

// Constructor
Tree::Tree() 
{
     root = NULL;
}

// Destructor
Tree::~Tree() 
{
     freeNode(root);
}

// Free the node and all of its children by recursion
void Tree::freeNode(Node* leaf)
{
    if ( leaf != NULL )
    {
       freeNode(leaf->Left());
       freeNode(leaf->Right());
       delete leaf;
    }
}

// Add a node
// if root doesn't exist create it, 
// otherwise send the key and root to other addNode function
void Tree::addNode(contact key) 
{
     // No elements. Add the root
     if ( root == NULL ) 
     {
        cout<<"add root node ... Name: "<<key.name
            <<" Phone Number: "<<key.phoneNumber<<endl;
        Node* n = new Node();
        n->setKey(key);
        root = n;
     }
     else 
     {
       cout<<"add other node ... Name: "<<key.name
           <<" Phone Number: "<<key.phoneNumber<<endl;
       addNode(key, root);
     }
}

// Add a node (private)
// checking for less than or equal to for left and
// greater than for right and passing the key and new root
// recursively if a subtree root already exists.
void Tree::addNode( contact key, Node* leaf) 
{
    if ( key.phoneNumber <= leaf->KeyPN() ) 
    {
       if ( leaf->Left() != NULL )
          addNode(key, leaf->Left());
       else 
       {
          Node* n = new Node();
          n->setKey(key);
          leaf->setLeft(n);
       }
    }
    else 
    {
       if ( leaf->Right() != NULL )
          addNode(key, leaf->Right());
       else 
       {
          Node* n = new Node();
          n->setKey(key);
          leaf->setRight(n);
       }
    }
}

// search for a single node
void Tree::search_bst(Node* n, int leaf)
{
   if ( n != NULL )
   {      
      if ( leaf < n->KeyPN() )
      {
         search_bst( n->Left(), leaf);
      }else if ( leaf > n->KeyPN() )
      {
         search_bst( n->Right(), leaf);
      }else
      {
         contact key = n->Key();
         cout<<"\n"<<key.name<<" found under phone number: "<<key.phoneNumber<<".\n";
      }  
   }
}

// Free a single node and move the rest of the tree into form
void Tree::delete_node(Node* n, int leaf)
{
    if ( n != NULL )
   {      
      if ( leaf < n->KeyPN() )
      {
         delete_node( n->Left(), leaf);
      }else if ( leaf > n->KeyPN() )
      {
         delete_node( n->Right(), leaf);
      }else
      {
          if ( n->Left() == NULL && n->Right() == NULL )
          {
             cout<<"\n"<<n->KeyName()<<" deleted.\n";
             delete n;
          }else if ( n->Left() != NULL && n->Right() == NULL )
          {
             Node temp = *n->Left();
             delete n->Left();
             n = temp;
             n->setLeft(NULL);
          }else if ( n->Left() == NULL && n->Right() != NULL )
          {
             Node temp = *n->Right();
             delete n->Right();
             n = temp;
             n->setRight(NULL);
          }else
          {
             Node* temp = lowest(n->Right());
             n->Key() = temp->Key();
             delete_node( n->Right(), temp->KeyPN() );
          }
      }
   }
}

Node* Tree::lowest(Node* n)
{
   if ( n->Left() != NULL )
   {
      lowest( n->Left() );
   }else
   {
      return n;
   }
   
   return n;
}

// Print the tree in-order
// Traverse the left sub-tree, root, right sub-tree
void Tree::inOrder(Node* n) 
{
    if ( n ) 
    {
       inOrder(n->Left());
       cout<<"Name: "<<n->KeyName()<<" \nPhone Number: "<<n->KeyPN()<<" \n";
       inOrder(n->Right());
    }
}

// Print the tree pre-order
// Traverse the root, left sub-tree, right sub-tree
void Tree::preOrder(Node* n) 
{
    if ( n ) 
    {
       //writting to file
       ofstream out_file;
       out_file.open("tree.dat");
       if(out_file.fail())
       { cout<<"Could not open file. Terminating program.\n"; }
       else
       {
          out_file<<"Name: "<<n->KeyName()<<" \nPhone Number: "<<n->KeyPN()<<" \n";
       }
       out_file.close();
       
       preOrder(n->Left());
       preOrder(n->Right());
    }
}

// Test main program
int main() 
{
   int choice = 0;
   int key = 0;
   contact newContact;

   Tree* tree = new Tree();

   while(choice != 5)
   {

      cout<<"\nChoose an option:\n";
      cout<<"1. Add a contact\n";
      cout<<"2. Delete a contact (by phone number)\n";
      cout<<"3. Find a contact (by phone number)\n";
      cout<<"4. Print out contacts (in numerical order by phone number)\n";
      cout<<"5. Quit (saves tree to file)\n";

      while(choice < 1 || choice > 5) { cin>>choice; }

      if(choice == 1)
      {
         cout<<"Enter a name: ";

         while( newContact.name == "" ) { getline(cin, newContact.name); }

         cout<<"\nEnter a phone number: ";

         while( newContact.phoneNumber == 0 ) { cin>>newContact.phoneNumber; }

         cout<<endl;
      
         tree->addNode( newContact );
      }
      
      if(choice == 2)
      {
         cout<<"Enter the phone number of the contact you wish to delete: ";
         while( key == 0 ) { cin>>key; }

         tree->delete_node( tree->Root(), key );
      }

      if(choice == 3)
      {
         cout<<"Enter the phone number of the contact you wish to find: ";
         while( key == 0 ) { cin>>key; }

         tree->search_bst( tree->Root(), key );
      }

      if(choice == 4)
      {
         cout<<"Printing tree in ascending numerical order: \n";
         tree->inOrder( tree->Root() );
         cout<<endl;
      }
   
      if(choice == 5)
      {
         cout<<"Terminating program and saving tree to file tree.dat in pre-order";

         tree->preOrder( tree->Root() );
         cout<<endl;
      }

      if( choice != 5 ) { choice = 0; }
      newContact.name = "";
      newContact.phoneNumber = 0;
      key = 0;
   }

   delete tree;
   return 0;
}

