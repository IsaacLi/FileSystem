#include "Dir.h"
using namespace std;
/*	RootDir Methods		*/

/*	The print method for an individual file, used in the list root method
	to print each node of the linked list individually	*/
void FileNode::Print() {
	cout << "Name: " << this->fileName << "\tSize: " << this->fileSize << "\tLocation: " << this->fileLoc << "\n";
}

/* RootDir Constructor	*/
Dir::Dir() {
	
	this->pHead = NULL; 
	this->count = 0;
	this->headerLoc = ROOT_ADDRESS;
	/*	Zero fill rootHeader	*/
	strcpy(this->rootHeader.name, "root");
	this->rootHeader.size = 0;
	this->rootHeader.type = 1;
	for (int i = 0; i < ROOT_HEADER_SIZE; i++)
		this->rootHeader.dataBlocks[i] = 0;
}

Dir::Dir(blockNum headerLocation, int discIDNum)
{
	this->rootHeader.name[0] = '\0';
	this->pHead = NULL;
	this->count = 0;
	this->rootHeader.size = 0;
	this->rootHeader.type = 1;
	this->headerLoc = headerLocation;
	this->setDiscID(discIDNum);
	for (int i = 0; i < ROOT_HEADER_SIZE; i++)
		this->rootHeader.dataBlocks[i] = 0;
}


/*	Add file information to linked list representation of the root directory
	also add new file block number to hardware version of the root directory
	return false on error	*/
bool Dir::AddFile (string name, int size, blockNum location) {
	if (FindFile(name) == -1) {
		FileNode* pNewFile = new FileNode (name, size, location);
		FileHeader tempdata;
		readblock(this->discID, location, &tempdata);
		pNewFile->type = tempdata.type;//标识这个FileNode是File还是Directory
		pNewFile->pNext = this->pHead;
		this->pHead = pNewFile;
		/*	Add to rootHeader (hardware version of root)	*/
		this->rootHeader.dataBlocks[this->count] = location;
		this->count++;
		return true;
	}
	else
		return false;
}

bool Dir::AddDir(string name, blockNum location)
{
	if (FindFile(name) == -1) {
		//fix the size = 1
		FileNode *pNewFile = new FileNode(name, 0, location);
		pNewFile->pNext = pHead;
		pNewFile->type = 1;//type为1，表示这是一个directory，而不是file
		this->pHead = pNewFile;
		/*	Add to rootHeader (hardware version of root)	*/
		this->rootHeader.dataBlocks[this->count] = location;
		this->count++;
		return true;
	}
	else
		return false;
}



/*	Add file information to linked list representation of the root directory
	also add new file block number to hardware version of the root directory
	return false on error	*/
bool Dir::AddFile(blockNum location) {
	if (FindFile(location) == -1) {
		File* pFile = new File(location, this->discID);
		pFile->Load();
		//相当于是从硬盘获取的name, size
		FileNode* pNewFile = new FileNode (pFile->GetName(), pFile->GetSize(), pFile->GetHeaderLoc());
		pNewFile->pNext = pHead;
		this->pHead = pNewFile;
		/*	Add to rootHeader (hardware version of root)	*/
		this->rootHeader.dataBlocks[this->count] = location;
		this->count++;
		delete pFile;
		return true;
	}
	else
		return false;
}

bool Dir::AddDir(blockNum location) {
	if (FindFile(location) == -1) {
		Dir *pDir = new Dir(location, this->discID);
		pDir->LoadRoot();
		FileNode *pNewDir = new FileNode(pDir->GetName(), pDir->GetSize(), pDir->GetHeaderLoc());
		pNewDir->type = 1;
		pNewDir->pNext = pHead;
		this->pHead = pNewDir;

		this->rootHeader.dataBlocks[this->count] = location;
		this->count++;
		delete pDir;
		return true;
	}
	else
		return false;
}

/*	Iterate through the linked list and remove the node matching file name
	Also iterate through rootHeader and remove the entry for file location	*/
bool Dir::RemoveFile (string fileToRemove) {
	if (this->count == 0)
		return false;
	else {
		FileNode* pCurr = this->pHead;
		FileNode* pPrev = this->pHead;
		while (pCurr->fileName != fileToRemove) {
			if (pCurr->pNext == NULL)
				return false;
			else {
				pPrev = pCurr;
				pCurr = pCurr->pNext;
			}
		}
		if (pCurr == this->pHead)
			this->pHead = this->pHead->pNext;
		else
			pPrev->pNext = pCurr->pNext;
		/*	 Removed from list, now remove from rootHeader	*/
		int i;
		int value = pCurr->fileLoc;
		delete pCurr;
		for (i = 0; i < this->count; i++) 
			if (value == this->rootHeader.dataBlocks[i])
				break;
		if (i == this->count) {
			cout << "Error removing from root header, block location not found.\n";
			return false;
		}
		else {
			for (int j = i; j < this->count; j++)
				this->rootHeader.dataBlocks[j] = this->rootHeader.dataBlocks[j+1];
			this->count--;
			return true;
		}
	}
}
/*	Save the hardware version of the root directory to disc using writeblock function from return false on error */
bool Dir::SaveRoot () {
	this->rootHeader.size = this->count;
	int success = writeblock(this->discID, this->headerLoc, &(this->rootHeader));
	if (success == 1)
		return true;
	else
		return false;
}
/*	Load the hardware version of the root directory into memory,
	and add each file to the linked list representation of the root directory	*/
bool Dir::LoadRoot () {
	int success = 0;
	//cout << "163:" << this->rootHeader.name << endl;
	if (strcmp(this->rootHeader.name, "root") == 0) {
		success = readblock(this->discID, ROOT_ADDRESS, &(this->rootHeader));
		this->headerLoc = ROOT_ADDRESS;
	}
	else 
		success = readblock(this->discID, this->headerLoc, &(this->rootHeader));
	if (success) {
		cout << "count = " << count << endl;
		this->count = this->rootHeader.size;
		for (int i = 0; i < this->count; i++) {
			if (this->rootHeader.dataBlocks[i] > 2) {
				this->AddFile(this->rootHeader.dataBlocks[i]);
				this->count--;
			}
		}
		bool saved = this->SaveRoot();
		if (saved == false) {
			cout << "Error saving DIR to disc\n";
			return false;
		}
		return true;
	}
	else
		return false;
}
/*	Iterate through the linked list representation of the root directory,
	printing the information from each node until the end of the list	*/
void Dir::Print() const {
	cout << this->rootHeader.name << " Directory contains " << this->count << " entries\n";
	cout << "----------------------------------------\n";
	FileNode* pCurr = this->pHead;
	while (pCurr != NULL) {
		pCurr->Print();
		pCurr = pCurr->pNext;
	}
}

/*	Iterate through the linked list and find the file using the file name 
	as the search key. Return the block number for the file if found, Otherwise
	return -1	*/ 
blockNum Dir::FindFile(string searchKey) const {
	if (this->count == 0)
		return -1;
	else {
		FileNode* pCurr = this->pHead;
		while ( pCurr->fileName != searchKey) {
			if (pCurr->pNext == NULL)
				return -1;
			else
				pCurr = pCurr->pNext;
		}
		return pCurr->fileLoc;
	}
}

/*	Iterate through the linked list and find the file using the file name 
	as the search key. Return the block number for the file if found, Otherwise
	return -1	*/ 
blockNum Dir::FindFile(blockNum searchKey) const {
	if (this->count == 0 || this->pHead == NULL)
		return -1;
	else {
		FileNode* pCurr = this->pHead;
		while (pCurr->fileLoc != searchKey) {
			if (pCurr->pNext == NULL)
				return -1;
			else
				pCurr = pCurr->pNext;
		}
		return pCurr->fileLoc;
	}
}

void Dir::ClearFileNode()
{
	FileNode *pFileNode = this->pHead;
	while (pFileNode) {
		FileNode *temp = pFileNode->pNext;
		delete pFileNode;
		pFileNode = temp;
	}
}
