A commandline tool to randomly generate an xml file compilant with a pseudo-dtd definition.
As this is a toy project I wrote while learning C++, the code is currently very buggy and already in need of refactoring

Note: This tool is by no means complete. The examples below are a good indicator of
all the features that have been implemented.

## Motivation
During the Databases2 exam in polimi we are given the structure of an xml document
in a pseudo-dtd format and we are asked to write the xquery code to query some data.

This program can be used to automatically generate xml code to practice with the exam.
And, although this program can't generate data with the exact semantics, its output
can be useful as a starting point and manually edited. 

## How to use
### Basic usage
Input: (note the addition of the "root[]" identifier and of the quantifiers wrt. to the usual DTD format)
```
<Element root[] Library(Book[2])>
<Element Book(Title, Description, Author[3])>
<Element Author(Name, Surname)>
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
In addition to the usual DTD format, this program allows you to define where some 
of the elements can be reused, through the concepts of Pools and ResourceRequests

In the following example we declare Author as a pool of 10 unique elements. Each Author will
then be randomly drawn from the pool when needed.

Input:
```
<Element root[] Library(Book[2])>
<Element Book(Title, Description, Author[2])>
<Element pool[10] Author(Name, Surname)>
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

In the Group line we are specifiyng that "all Group elements within the same SummerCamp will
draw Child elements from a pool of 17 Children (in turn taken from the parent SummerCamp's 
Child Elements pool), and the set of Child for each Group will be distinct"

## Input Specification
Input must be a list of elements in this format:

<Element kind[quantifier] Name(ListOfChildren) has ResourceRequestsList>

where:
* kind can be one of pool, element, and root
* ListOfChildren is a list fo elements in this format Name or Name[quantifier], where absence of quantifier implies 1
* ResourceRequestsList is a list of elements in this format: shared Name[quantifier] or distinct Name[quantifier] 

there must be at least one root[] element (the last one is considered if multiple are provided).

The program tries to be as permissive as possible as for the syntax (for example the initial delimiter), but
it doesn't currently give any error. Please check the diagnostic output on stderror
to see if the input has been parsed properly).