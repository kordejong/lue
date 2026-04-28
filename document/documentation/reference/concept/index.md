(reference-concept)=

# Concepts

A concept is a name for a set of type requirements. A concrete type adhering to these requirements is said to
_model_ the concept. For example, an `Array` concept can be defined in terms of requirements like this (there
will be more requirements):

- There must be a nested type called `Element`, defining the type of the elements
- There must be a member function called `rank`, returning the dimensionality as an integral
- There must be a member function called `extents`, returning a collection of extents in each dimension

Depending on the programming language, it may be possible to enforce concepts. In C++ concepts can be used to
constrain type parameters of class and function templates. The template can then only be instantiated by types
modelling the concept, and if they don't, a relevant error message can be shown by the compiler. In Python,
concepts are not a thing, but [type hints(https://docs.python.org/3/library/typing.html) and duck typing are
related.

In any case, concepts can be documented, and used to describe types that can be passed into functions, for
example. That is what these pages are intended for. Since LUE contains operations and types that can be used
from multiple programming languages, instead of documenting each language specific API, duplicating a lot of
text, the LUE documentation is written in terms of language agnostic operations, concepts, and types modelling
these concepts. Since examples are included, for the various supported programming languages, it is hoped that
the user can infer how to use LUE operations from a specific language.

Each page in this section describes a concept and the types modelling these concepts. In the reference pages
for operations, signatures are shown in terms of concepts. Additionally, information is provided about which
concrete types can be used.

```{note}
Concepts in C++ make it possible to implement LUE in terms of types it does not know about. As long as those
types adhere to the requirements of the concepts used by LUE, all should be fine. This makes it possible, for
example, to compile a LUE version that uses types for representing rasters that are part of other software
packages, like [QGIS](https://qgis.org). This allows LUE to be integrated very tightly into other packages.
```
