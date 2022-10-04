# RDBMS

## QUERIES SUPPORTED BY SYSTEM
1. SHOW Tables
2. CREATE Table
3. INSERT INTO Table
4. DISPLAY Table contents
5. SEARCH INTO Table
6. DROP Table

Features:

#### 1. Custom Blocks for each table:
The data is stored in blocks.  

All the separate row data of a particular table is stored in different file creating blocks of data. System reads as well as writes data back to files in blocks.

#### 2. B+ Tree Indexing:
A separate multilevel B+ tree is created for each table. Each node contains maximum of 50 entries. Right now data storage  is based only on primary key, primary key can be INTEGER or VARCHAR. By default, first column is choosen as primary key.

#### 3. Storing Meta Data of Tables:
Meta Data of the tables are stored separately which provides quick access to several information like current number of records, table name, etc.
 
#### 4. Search:

##### 1. B+ Indexed Search
For searching a record in file for particular table, B+ tree indexed search is implemented. B+ indexed search is based on primary key
 
##### 2. Brute Force Search
For searching records without providing primary key from table, brute force search algorithm is implemented. You can display selected number of columns and limit results based on where clause(see section In Development below).
 
## DEVELOPMENT:

For execution: Open directory Relational_DBMS(branch:master) and on terminal

1. Type **make clean** to clean all binary files.
	
2. Type **make**, it will compile the project.
	
3. Type **./Relational_DBMS -u user -p** for running project. Password is `pass`
