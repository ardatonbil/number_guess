//Arda Baris Tonbil 31130

#include <iostream>
#include "feedback.h"
using namespace std;

struct node {  //taken from hw2 guide
    int digit;
    node * next;
};

int linked_list_length(node * head){  // used to measure the length of the linked list with a specific head
    int count = 0;
    while (head != nullptr){
        count++;
        head = head->next;
    }
    return count;
}

int count_linked_list(node *head, int element){  // used to count the occurence of a specific integer in a linked list
    int count = 0;
    while (head != nullptr){
        if (head->digit == element){
            count++;  //if it occurs then the count should increase by 1
        }
        head = head->next;
    }
    return count;
}

node * locate_linked_list(node *head, int element){  // used to locate the node of a specific integer
    while(head != nullptr){
        if (head->digit == element){
            node * temp = head;
            return temp;
        }
        head = head->next;
    }
    return head; //if it doesnt occur then the function should return head because if it returns nullptr, the code crashes
}

/* Begin: code taken from ptrfunc.cpp and updated*/

node * AddInOrder(node * head, int newkey)
// pre: list is sorted
// post: add newkey to list, keep list sorted, return head of new list with newkey in it
{
    node *  ptr = head;   // loop variable

    // if new node should be first, handle this case and return
    // in this case, we return the address of new node since it is new head
    if (head == nullptr || newkey < head->digit){
        node * temp = new node;  //node to be inserted
        temp->digit = newkey;
        temp->next = head; //connect the rest
        return temp;
        //return new node(newkey, head);    //alternative to prev. 4 lines
    }

    // check node one ahead so we don't pass!
    while (ptr->next != nullptr && ptr->next->digit < newkey)
    {
        ptr = ptr->next;
    } // postcondition: new node to be inserted just after the node that ptr points

    //now insert node with newkey after where ptr points to
    if (count_linked_list(ptr, newkey) == 0){
        node * temp = new node;  //node to be inserted
        temp->digit = newkey;
        temp->next = ptr->next; //connect the rest
        ptr->next = temp;
    }
    return head;
}



void DisplayList (node * head)
{
    while (head != nullptr)
    {
        cout << head->digit << " ";
        head = head->next;
    }
}

void DeleteOneNode (node * toBeDeleted, node * & head)
/* pre: toBeDeleted points to the node to be deleted from the list
   post: deletes the node pointed by toBeDeleted, updates head if changes */
{
    if (toBeDeleted != nullptr && head != nullptr){
        node * ptr;

        if (toBeDeleted == head)  //if the node to be deleted is the first node
        {
            head = head->next;
            delete toBeDeleted;
        }
        else  //if the node to be deleted is in the middle or at the end
        {
            ptr = head;
            while (ptr->next != toBeDeleted && ptr != nullptr)
                ptr = ptr->next;
            //after while ptr point to the node just before toBeDeleted

            //connect the previous node to the next node and delete
            ptr->next = toBeDeleted->next;
            delete toBeDeleted;
        }
    }

}

/* End: code taken from ptrfunc.cpp and updated*/



int main() {
    node *slot1_p = nullptr, *slot2_p = nullptr, *slot3_p = nullptr;
    node *slot1_imp = nullptr, *slot2_imp = nullptr, *slot3_imp = nullptr;

    node *possibles[3];    //array of pointers
    possibles[0] = slot1_p;
    possibles[1] = slot2_p;
    possibles[2] = slot3_p;

    node *impossibles[3];  //array of pointers
    impossibles[0] = slot1_imp;
    impossibles[1] = slot2_imp;
    impossibles[2] = slot3_imp;

    int game_id;
    cout << "Please enter a game ID.\n";
    cin >> game_id;

    while (true) {
        string guess;
        cout << "Enter your guess.\n";
        cin >> guess;
        if ((guess.length() != 3) || (guess[0] == guess[1]) || (guess[1] == guess[2]) || (guess[0] == guess[2]) ||
            (int(guess[0]) < 48) || (int(guess[0]) > 57) || (int(guess[1]) < 48) || (int(guess[1]) > 57) ||
            (int(guess[2]) < 48) || (int(guess[2]) > 57)) {
            //all possible cases for wrong guess inputs (wrong length, wrong characters, same numbers at different digits)
            cout << "Invalid guess. ";
            continue;
        }
        else {
            string feedback = get_the_feedback(guess, game_id);
            if (feedback[0] == 'R') { //wrong for every slot
                impossibles[0] = AddInOrder(impossibles[0], int(guess[0]) - 48);
                impossibles[1] = AddInOrder(impossibles[1], int(guess[0]) - 48);
                impossibles[2] = AddInOrder(impossibles[2], int(guess[0]) - 48);
            }
            else if (feedback[0] == 'Y') {  //wrong for spot 0 but possible for others
                impossibles[0] = AddInOrder(impossibles[0], int(guess[0]) - 48);
                if (count_linked_list(impossibles[1],int(guess[0]) - 48) == 0){
                    //if it is in impossibles, it shouldnt be added in possibles
                    possibles[1] = AddInOrder(possibles[1], int(guess[0]) - 48);
                }
                if (count_linked_list(impossibles[2],int(guess[0]) - 48) == 0){
                    possibles[2] = AddInOrder(possibles[2], int(guess[0]) - 48);
                }
                DeleteOneNode(locate_linked_list(possibles[0], int(guess[0]) - 48), possibles[0]);
            }
            else if (feedback[0] == 'G') { //correct spot (wrong for other spots because the digits are distinct
                possibles[0] = AddInOrder(possibles[0], int(guess[0]) - 48);
                impossibles[1] = AddInOrder(impossibles[1], int(guess[0]) - 48);
                impossibles[2] = AddInOrder(impossibles[2], int(guess[0]) - 48);
                DeleteOneNode(locate_linked_list(impossibles[0], int(guess[0]) - 48), impossibles[0]);
                DeleteOneNode(locate_linked_list(possibles[1], int(guess[0]) - 48), possibles[1]);
                DeleteOneNode(locate_linked_list(possibles[2], int(guess[0]) - 48), possibles[2]);
                for (int i = 0; i < 10; i++) {
                    if (i != int(guess[0]) - 48) {  //adding other digits to impossibles
                        impossibles[0] = AddInOrder(impossibles[0], i);
                    }
                }
                for (int i = 0; i < 10; i++) {
                    if (i != int(guess[0]) - 48) {  //the correct number is found so others wont be possible
                        DeleteOneNode(locate_linked_list(possibles[0], i), possibles[0]);
                    }
                }
            }


            if (feedback[1] == 'R') {  //same explanations with spot 0
                impossibles[0] = AddInOrder(impossibles[0], int(guess[1]) - 48);
                impossibles[1] = AddInOrder(impossibles[1], int(guess[1]) - 48);
                impossibles[2] = AddInOrder(impossibles[2], int(guess[1]) - 48);
            }
            else if (feedback[1] == 'Y') {
                impossibles[1] = AddInOrder(impossibles[1], int(guess[1]) - 48);
                DeleteOneNode(locate_linked_list(possibles[1], int(guess[1]) - 48), possibles[1]);
                if (count_linked_list(impossibles[0],int(guess[1]) - 48) == 0){
                    possibles[0] = AddInOrder(possibles[0], int(guess[1]) - 48);
                }
                if (count_linked_list(impossibles[2],int(guess[1]) - 48) == 0){
                    possibles[2] = AddInOrder(possibles[2], int(guess[1]) - 48);
                }
            }
            else if (feedback[1] == 'G') {
                possibles[1] = AddInOrder(possibles[1], int(guess[1]) - 48);
                impossibles[0] = AddInOrder(impossibles[0], int(guess[1]) - 48);
                impossibles[2] = AddInOrder(impossibles[2], int(guess[1]) - 48);
                DeleteOneNode(locate_linked_list(impossibles[1], int(guess[1]) - 48), impossibles[1]);
                DeleteOneNode(locate_linked_list(possibles[0], int(guess[1]) - 48), possibles[0]);
                DeleteOneNode(locate_linked_list(possibles[2], int(guess[1]) - 48), possibles[2]);
                for (int i = 0; i < 10; i++) {
                    if (i != int(guess[1]) - 48) {
                        impossibles[1] = AddInOrder(impossibles[1], i);
                    }
                }
                for (int i = 0; i < 10; i++) {
                    if (i != int(guess[1]) - 48) {
                        DeleteOneNode(locate_linked_list(possibles[1], i), possibles[1]);
                    }
                }
            }


            if (feedback[2] == 'R') {  //same with spot 0
                impossibles[0] = AddInOrder(impossibles[0], int(guess[2]) - 48);
                impossibles[1] = AddInOrder(impossibles[1], int(guess[2]) - 48);
                impossibles[2] = AddInOrder(impossibles[2], int(guess[2]) - 48);
            }
            else if (feedback[2] == 'Y') {
                impossibles[2] = AddInOrder(impossibles[2], int(guess[2]) - 48);
                if (count_linked_list(impossibles[0],int(guess[2]) - 48) == 0){
                    possibles[0] = AddInOrder(possibles[0], int(guess[2]) - 48);
                }
                if (count_linked_list(impossibles[1],int(guess[2]) - 48) == 0){
                    possibles[1] = AddInOrder(possibles[1], int(guess[2]) - 48);
                }
                DeleteOneNode(locate_linked_list(possibles[2], int(guess[2]) - 48), possibles[2]);
            }
            else if (feedback[2] == 'G') {
                possibles[2] = AddInOrder(possibles[2], int(guess[2]) - 48);
                impossibles[0] = AddInOrder(impossibles[0], int(guess[2]) - 48);
                impossibles[1] = AddInOrder(impossibles[1], int(guess[2]) - 48);
                DeleteOneNode(locate_linked_list(impossibles[2], int(guess[2]) - 48), impossibles[2]);
                DeleteOneNode(locate_linked_list(possibles[0], int(guess[2]) - 48), possibles[0]);
                DeleteOneNode(locate_linked_list(possibles[1], int(guess[2]) - 48), possibles[1]);
                for (int i = 0; i < 10; i++) {
                    if (i != int(guess[2]) - 48) {
                        impossibles[2] = AddInOrder(impossibles[2], i);
                    }
                }
                for (int i = 0; i < 10; i++) {
                    if (i != int(guess[2]) - 48) {
                        DeleteOneNode(locate_linked_list(possibles[2], i), possibles[2]);
                    }
                }
            }
        }
        cout << "Linked lists:\nSlot: 1\nImpossibles: ";
        DisplayList(impossibles[0]);
        cout << "\nPossibles: ";
        DisplayList(possibles[0]);
        cout << "\n\nSlot: 2\nImpossibles: ";
        DisplayList(impossibles[1]);
        cout << "\nPossibles: ";
        DisplayList(possibles[1]);
        cout << "\n\nSlot: 3\nImpossibles: ";
        DisplayList(impossibles[2]);
        cout << "\nPossibles: ";
        DisplayList(possibles[2]);
        cout << "\n\n";

        if (linked_list_length(impossibles[0]) == 9 && linked_list_length(impossibles[1]) == 9 && linked_list_length(impossibles[2]) == 9){
            //ending phase
            int secretnumber = 0;
            for (int i = 0; i < 10; i++){
                if (count_linked_list(impossibles[0], i) == 0){
                    secretnumber+=i*100;
                }
                if (count_linked_list(impossibles[1], i) == 0){
                    secretnumber+=i*10;
                }
                if (count_linked_list(impossibles[2], i) == 0){
                    secretnumber+=i;
                }
            }
            if (secretnumber >= 100){
                cout << "The secret number is: " << secretnumber << "\nCongrats! Now, deleting the lists...";
            }
            else{ //if the value is lower than 100 then spot 0 should be 0 so I added it as string character
                cout << "The secret number is: 0" << secretnumber << "\nCongrats! Now, deleting the lists...";
            }

            //starting to deallocate the lists and to free the memory  (all while loops after this line)
            while (impossibles[0] != nullptr){
                DeleteOneNode(impossibles[0]->next, impossibles[0]);
                if (impossibles[0]->next == nullptr){
                    DeleteOneNode(impossibles[0], impossibles[0]);
                }
            }
            while (impossibles[1] != nullptr){
                DeleteOneNode(impossibles[1]->next, impossibles[1]);
                if (impossibles[1]->next == nullptr){
                    DeleteOneNode(impossibles[1], impossibles[1]);
                }
            }
            while (impossibles[2] != nullptr){
                DeleteOneNode(impossibles[2]->next, impossibles[2]);
                if (impossibles[2]->next == nullptr){
                    DeleteOneNode(impossibles[2], impossibles[2]);
                }
            }
            while (possibles[0] != nullptr){
                DeleteOneNode(possibles[0]->next, possibles[0]);
                if (possibles[0]->next == nullptr){
                    DeleteOneNode(possibles[0], possibles[0]);
                }
            }
            while (possibles[1] != nullptr){
                DeleteOneNode(possibles[1]->next, possibles[1]);
                if (possibles[1]->next == nullptr){
                    DeleteOneNode(possibles[1], possibles[1]);
                }
            }
            while (possibles[2] != nullptr){
                DeleteOneNode(possibles[2]->next, possibles[2]);
                if (possibles[2]->next == nullptr){
                    DeleteOneNode(possibles[2], possibles[2]);
                }
            }
            return 0;
        }
    }
}
