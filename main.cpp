#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include <google/sparse_hash_map>
//#include <tr1/functional>

#include "gliamodels.h"
#include "nodealign.h"
#include "traceback.h"
#include "gsw.h"
#include "examples.h"
#include "fastq.h"
#include "show.h"
#include "ghash.h"
#include "getSeeds.h"


using namespace std;


string reverseComplement(string read) {
	
	// Declare the reverse complement read as a string
	string rc_read;	
	
	// Reverse Read
	rc_read.assign(read.rbegin(), read.rend());
	
	// Complement.  Note that not IUPAC compliant. Uses the alphabet {A,T,C,G,N}
	string::iterator t;
	for (t = rc_read.begin(); t != rc_read.end(); ++t) {
		switch (*t) {
			case 'A':
				*t = 'T';
				break;
			case 'T':
				*t = 'A';
				break;
			case 'C':
				*t = 'G';
				break;
			case 'G':
				*t = 'C';
				break;
			case 'N':
				*t = 'N';
				break;
			default:
				cout << "Unknown Nucleotide!";
				break;
		}
	}
	
	// Return the Read  (faster if done through pointers?)
	return rc_read;
}

// TODO: Move to GHASH
int hashfasta(string fasta_file_name, int hashsize, vector<fasta_entry> &ref_genome) {	

		
	// map<string, vector<int> > contig_hash;
	// google::sparse_hash_map<string, vector <int>, tr1::hash<string> > contig_hash;
	/* Disabled after moving the hash to the fasta_entry struct
	google::sparse_hash_map<string, vector <int> > contig_hash;
	contig_hash.set_deleted_key("");
	*/	

	load_fasta_file(fasta_file_name, ref_genome);
	
	vector<fasta_entry>::iterator t;
	
	for (t = ref_genome.begin(); t != ref_genome.end(); ++t) {
		cout << t->name << endl;
		//cout << t->sequence << endl;
		hashcontig(*t, t->kmer_hash, hashsize);
		cout << t->name << " ...hash complete" << endl;
	
		sortContigHash(t->kmer_hash);
		cout << t->name << " ...sort complete" << endl;
				
	}
	
	return 0;
}
	

int main (int argc, char * const argv[])
{
	
	/* ___ PART 1:  Hash the Genome "Build Step" __________________  */
	
    
    // The File Path of the Reference Genome (JSON?)
	string fasta_file_name = "/Users/kural/Downloads/chr20.fa";
	
	// The Reference Genome (a vector of fasta entries
	vector<fasta_entry> ref_genome;	
	
	// kmer hash build size.. very important (JSON?)
	int hashsize = 25;

	/* Loads up the genome & hashes it. Might carve out in the future as 
	   a build step, after a binary serialization is done 
	   (i.e. can write out a "processed genome build"  */
	hashfasta(fasta_file_name, hashsize, ref_genome);


	
	/* ___ PART 2:  Read FASTQ and Align Reads _____________________ */

	string read = "CTTCTTCTTCTTCTTCTTCTTCTTCCTTCTTCTTCTTCTTCTTCTTCTTC";
		
	// Declere a read object from a fastq file
	fastq_entry read_q;					
	
	// Open FastQ File
	ifstream filehandle ("test.fastq");		// open fastq file
	
	// Main input file loop
	if (filehandle.is_open()) 
	{
		while (filehandle.good()) 
		{

			// Load up the read with the four lines from the FastQ file
			read_q = getNextRead(filehandle);
			cout << read_q.readname << endl;
			
			// Reverse Complement the sequence information of the read.
			read_q.rc_sequence = reverseComplement(read_q.sequence);

			
			vector<fasta_entry>::iterator t;
			for (t = ref_genome.begin(); t != ref_genome.end(); ++t) {
				
				// vector<pair<int, int> > clusters;

				lookupRead(read, t->kmer_hash, hashsize);
			}
			
			
			/* --- alignment -- */
			
			// string read = "ATCGAA";
			
			
			// Declare the target DAG to align against. 
			vector<sn*> nlist;
			origIndel(nlist);
			
			
			cout << "in the main:"<< endl<<nlist.front()->name << endl;
			sn* result = gsw(read, nlist);
			
			cout<<result->name<<", top score: "<<result->top_score.score<<endl;
			
			displayNode(result);
			displayAlignment(result);
			displayAlignment(nlist[0]);
			
			mbt trace_report;
			master_backtrack(result, trace_report);
			
			cout << "x: " << trace_report.x << " y: " << trace_report.y << endl;
			cout << trace_report.cigar << endl;
			
		}
		filehandle.close();
		
	}
	else cout << "Unable to open read file.\n"; 
	
	
	
	
	
	//write_file();
	

	
	return 0;
	
}

