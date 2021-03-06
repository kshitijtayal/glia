/*
 *  show.cpp
 *  glia
 *
 *  Created by Deniz Kural on 1/1/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "show.h"

using namespace std;


/*
vector<string> parentNames(sn* node) {
	
	vector<string> parentNames;
	
	vector<sn*>::iterator t;									// good place for vector iteration?
	for (t=node.p5.begin(); t!=node.p5.end(); ++t) {
		parent_names.push_back((*t)->name);
	}
	
	return parentNames;
}

int displayStrings(vector<string> values) {
	
	vector<sn*>::iterator t;									// good place for vector iteration?
	for (t=node.p5.begin(); t!=node.p5.end(); ++t) {
		parent_names.push_back((*t)->name);
	}
	
*/

int displayNode(sn* node) {

	cout << "Node Summary" << endl << "----------" << endl;
	cout << "NodeName: " << node->name << endl;
	cout << "Sequence: " << node->sequence << endl;
	cout << "Depth:    " << node->depth << endl;
	cout << "Parents:  ";
	
	vector<sn*>::iterator t;									
	
	for (t = node->p5.begin(); t != node->p5.end(); ++t) {
		cout<<(*t)->name<<", ";
	}
	cout<<endl;
	
	cout << "Children: ";
		
	for (t = node->p3.begin(); t != node->p3.end(); ++t) {
		cout<<(*t)->name<<", ";
	}
	cout<<endl<<endl;
	
	return 0;
}




int displayAlignment(sn* node, string& read) {
	
	vector<vector<ms> >::iterator y;
	vector<ms>::iterator x;

	cout << "Node: " << node->name << endl << endl;
	
	cout << "Score:"<< endl << "----------" << endl;

    cout << "\t\t";
    for (string::iterator s = node->sequence.begin(); s != node->sequence.end(); ++s) {
        cout << "\t" << *s;
    }
    cout << endl;

	for (y = node->matrix.begin(); y != node->matrix.end(); ++y) {
		cout << "\t" << read[y-node->matrix.begin()-1] << "\t";
		for (x = (*y).begin(); x != (*y).end();  ++x) {
		    cout << x->score << "\t";
		}
		cout << endl;
	}
	
	cout << endl;
	cout << "Back Trace:" << endl << "----------" << endl;
	
	for (y = node->matrix.begin(); y != node->matrix.end(); ++y) {
		cout << "\t";
		for (x = (*y).begin(); x != (*y).end();  ++x) {
		    cout << x->arrow << "\t";
		}
		cout << endl;
	}
	
	cout << endl;
	cout << "Parent:" << endl << "----------" << endl;
	
	for (y = node->matrix.begin(); y != node->matrix.end(); ++y) {
		cout << "\t";
		for (x = (*y).begin(); x != (*y).end();  ++x) {
		    cout << x->parent->name << "\t";
		}
		cout << endl;
	}

	cout << endl;
	return 0;
}
