#pragma once
#include <iostream>
using namespace std;

template <class K, class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	AVLTreeNode<K,V> *_left;
	AVLTreeNode<K,V> *_right;
	AVLTreeNode<K,V> *_parent;
	int _bf;//平衡因子，右子树高度-左子树的高度

	AVLTreeNode(const K& key, const V& value)
		:_key(key)
		,_value(value)
		,_bf(0)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
	{}

};

template <class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K,V> Node;

public:
	AVLTree()
		:_root(NULL)
	{}

	AVLTree(const AVLTree<K,V> &key)
		:_root(NULL)
	{
		_root=_Copy();
	}

	~AVLTree()  
	{  
		_Destory(_root);  
	}  

	AVLTree<K, V>& operator=(const AVLTree<K,V>& avl)  
	{  
		if (this != &avl)  
		{  
			AVLTree<K, V> tmp(avl);  
			swap(_root,tmp._root);  
		}  
		return *this;  
	}  

	bool Insert(const K key , const V value)
	{
		//空树
		if (_root == NULL)
		{
			_root= new Node(key, value);
				return true;
		}

		Node *parent=NULL;
		Node *cur = _root;
		while(cur)//判断key与cur中-key的值的大小，如果相等则返回false
		{
			
			if(cur->_key < key)
			{
				parent=cur;
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				parent=cur;
				cur = cur->_left;
			}
			else
				return false;
		}

			cur=new Node(key,value);
			cur->_parent=parent;
			if(parent->_key < key)
				parent->_right=cur;
			else                   //parent->_key > key
				parent->_left=cur;

			while(parent)
			{
				if (parent->_left==cur)
				{
					parent->_bf--;
				}
				else if (parent->_right==cur)
				{
					parent->_bf++;
				}

				if (parent->_bf==0)
				{
					//当前树的高度没变化，说明此时已经满足AVL树
					break;
				}
				else if (parent->_bf==1 || parent->_bf==-1)
				{
					//当前数的高度已经变了，继续向上调整
					cur=parent;
					parent=cur->_parent;
				}
				else //2/-2,已经不平衡，旋转
				{
					if (parent->_bf==2)
					{
						if (cur->_bf==1)
						{
							RotateL(parent);
						} 
						else      //cur->_bf==-1
						{
							RotateRL(parent);
						}
					}
					else         //parent->_bf==-2
					{
						if (cur->_bf==-1)
						{
							RotateR(parent);
						} 
						else
						{
							RotateLR(parent);
						}
					}
				}
			}
			

	}



	bool Find(const K& key)  
	{  
		Node* cur = _root;  
		while (cur)  
		{  
			if (cur->_key < key)  
				cur = cur->_right;  
			else if (cur->_key>key)  
				cur = cur->_left;  
			else  
				return true;  
		}  
		return false;  
	}  
	void InOder()  
	{  
		_InOder(_root);  
	}

	bool IsBalanceOP()  
	{  
		int height=0;  
		return _IsBalanceOP(_root,height);  
	}  

protected:
	Node *_Copy(Node *root)
	{
		Node * newnode=NULL;
		if (root!=NULL)
		{
			newnode = new Node(root->_value);
			newnode->_left=_Copy(root->_left);
			newnode->_right=_Copy(root->_right);
		}
		return newnode;
	}

	bool _IsBalanceOP(Node *root,int &height)     
	{  
		if(root == NULL)  
		{  
			height=0;  
			return true;  
		}  
		int leftheight=0;  
		if(_IsBalanceOP(root->_left,leftheight)==false)
			return false;
		int rightheight=0;  
		if(_IsBalanceOP(root->_right,rightheight)==false)
			return false;
		height = leftheight>rightheight? leftheight+1 : rightheight+1;
		return abs(rightheight-leftheight) < 2;
		
	}  

	void _Destory(Node* root)  
	{  
		if (root == NULL)  
			return;  
		_Destory(root->_left);  
		_Destory(root->_right);  
		delete root;  
	}  
	void _InOder(Node* root)  
	{  
		if (root == NULL)  
			return;  
		_InOder(root->_left);  
		cout << root->_key << " " << root->_bf << endl;  
		_InOder(root->_right);  
	}

	//右单旋
	void RotateR(Node *& parent )
	{
		Node *subL=parent->_left;
		Node *subLR=subL->_right;
		parent->_left=subLR;
		if (subLR)
			subLR->_parent = parent;
			subL->_right = parent;
			Node *ppNode = parent->_parent;
			parent->_parent = subL;

			if (ppNode == NULL)
			{
				_root= subL;
				subL->_parent=NULL;
			} 
			else
			{
				if (ppNode->_right= parent)
					ppNode->_right = subL;
				else
					ppNode->_left = subL;
				subL->_parent = ppNode;
			}
			subL->_bf=parent->_bf=0;


	}

    //左单旋
    void RotateL(Node *& parent )
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		parent->_right = subRL;

		if (subRL)
			subRL->_parent = parent;
			subR->_left = parent;
			Node *ppNode = parent->_parent;
			parent->_parent = subR;

			if (ppNode == NULL)
			{
				_root = subR;
				subR->_parent = NULL;
			} 
			else
			{
				if (ppNode->_left == parent)
					ppNode->_left = subR;
				else
					ppNode->_right = subR;
				subR->_parent = ppNode;
			}
			subR->_bf = parent->_bf = 0;
	}
    //右左旋转
	void RotateRL(Node *& parent )
	{
		Node *subR=parent->_right;  
		Node *subRL=subR->_left;  
		int bf=subRL->_bf;  
		RotateR(parent->_right);  
		RotateL(parent);  
		if(bf == 0)  
			parent->_bf=subR->_bf=subRL->_bf=0;  
		else if(bf == 1)  
		{  
			subR->_bf=0;  
			subRL->_bf=1;  
			parent->_bf=-1;  
		}  
		else       //-1  
		{  
			subR->_bf=1;  
			subRL->_bf=-1;  
			parent->_bf=0;  
		}  
	}
    //左右旋转
	void RotateLR(Node *& parent )
	{
		Node *subL=parent->_left;  
		Node *subLR=subL->_right;  
		int bf=subLR->_bf;  
		RotateL(parent->_left);  
		RotateR(parent);  
		if(bf == 0)  
			subLR->_bf=subL->_bf=parent->_bf=0;  
		else if(bf == 1)  
		{  
			subL->_bf=-1;  
			subLR->_bf=1;  
			parent->_bf=0;  
		}  
		else         //-1  
		{  
			subL->_bf=0;  
			subLR->_bf=-1;  
			parent->_bf=1;  
		}  
	}
protected:
	Node *_root;
};

int main()
{
	int array1[]={16, 3, 7, 11, 9, 26, 18, 14, 15};  
	size_t size=sizeof(array1)/sizeof(array1[0]);  
	AVLTree<int,int> tree;  
	for (size_t i=0;i<size;++i)  
	{  
		tree.Insert(array1[i],i);  
	}  
	tree.InOder();  
	cout<<"tree IsBalance?"<<tree.IsBalanceOP()<<endl;   //1
	system("pause");
	return 0;

}