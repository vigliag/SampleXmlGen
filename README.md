A tool to randomly generate XML files according to a given structure given in a pseudo-DTD format.

Note: This is a toy project I wrote to practice C++, the code is currently memory-leaking 
and already in need of refactoring. The examples below are a good indicator of
all the implemented features.

A working demo of SampleXMLGen (obtained through emscripten) is available [here](http://vigliag.github.io/SampleXmlGen/)

### Motivation
Create fake/random data to practice xquery for the Databases2 exam in Politecnico di Milano. 
The language this tool accepts mirrors the one we are given during the exam.
The generated data is intended to be used as starting point and be manually edited. 

## How to use
### Basic usage
Input: (note the addition of the "root[]" identifier and of the quantifiers wrt. to the usual DTD format)
```
<!Element root[] Library(Book[2])>
<!Element Book(Title, Description, Author[3])>
<!Element Author(Name, Surname)>
```

Output:
```xml
<?xml version="1.0"?>
<Library>
        <Book>
                <Title>Library1Book1Title1</Title>
                <Description>Library1Book1Description1</Description>
                <Author>
                        <Name>Library1Book1Author1Name1</Name>
                        <Surname>Library1Book1Author1Surname1</Surname>
                </Author>
                <Author>
                        <Name>Library1Book1Author2Name1</Name>
                        <Surname>Library1Book1Author2Surname1</Surname>
                </Author>
        </Book>
        <Book>
                <Title>Library1Book2Title1</Title>
                <Description>Library1Book2Description1</Description>
                <Author>
                        <Name>Library1Book2Author1Name1</Name>
                        <Surname>Library1Book2Author1Surname1</Surname>
                </Author>
                <Author>
                        <Name>Library1Book2Author2Name1</Name>
                        <Surname>Library1Book2Author2Surname1</Surname>
                </Author>
        </Book>
</Library>
```

### Advanced usage
In addition to the usual pseudo-DTD format, this program allows you to define if some 
of the elements can be reused, through the concepts of Pools and ResourceRequests

In the following example we declare Author as a pool of 10 unique elements. 
During the main generation phase, the Author elements will be randomly drawn from the pool
(instead of being uniquely generated).

Input:
```
<!Element root[] Library(Book[2])>
<!Element Book(Title, Description, Author[2])>
<!Element pool[10] Author(Name, Surname)>
```
Output:
```xml
<?xml version="1.0"?>
<Library>
        <Book>
                <Title>Library1Book1Title1</Title>
                <Description>Library1Book1Description1</Description>
                <Author>
                        <Name>Author8Name1</Name>
                        <Surname>Author8Surname1</Surname>
                </Author>
                <Author>
                        <Name>Author9Name1</Name>
                        <Surname>Author9Surname1</Surname>
                </Author>
        </Book>
        <Book>
                <Title>Library1Book2Title1</Title>
                <Description>Library1Book2Description1</Description>
                <Author>
                        <Name>Author6Name1</Name>
                        <Surname>Author6Surname1</Surname>
                </Author>
                <Author>
                        <Name>Author10Name1</Name>
                        <Surname>Author10Surname1</Surname>
                </Author>
        </Book>
</Library>

```

### Complex example

Consider now this input:

```
<!ELEMENT root[] Organization ( SummerCamp[4] )>
<!ELEMENT SummerCamp ( Name, Educator[3], Group[2], City, Region, StartDate, EndDate ) has shared Child[22]>
<!ELEMENT pool[5] Educator ( SSN, Name, Surname, YearOfBirth )>
<!ELEMENT Group ( Name, SSNEducator, Child[6] ) has distinct Child[17]>
<!ELEMENT pool[60] Child ( SSN, Name, Surname, YearOfBirth )>
```

In the "Group" Definition, we are specifiyng that "all Group elements within the same SummerCamp will
draw Child elements from a pool of 17 Children (in turn taken from the parent SummerCamp's 
Child Elements pool), and the set of Child for each Group will be distinct"

## Input Specification
Note: this part of the documentation is incomplete. Feel free to contact me via mail

Input must be a list of elements in this format:

<Element kind[quantifier] Name(ListOfChildren) has ResourceRequestsList>

where:
* kind can be one of pool, element, and root
* ListOfChildren is a list of elements in this format Name or Name[quantifier]
* ResourceRequestsList is a list of elements in this format: shared Name[quantifier] or distinct Name[quantifier] 

there must be at least one root[] element (the last one is considered if multiple are provided).

While parsing, the program tries to be as permissive as possible (for example regarding the initial delimiter). Note that it
currently doesn't give any error. Please check the diagnostic output on stderror
to see if the input has been parsed properly).