(reference-concept)=

# Concepts

A concept is a name for set of type requirements. A concrete type adhering to these requirements is said to
_model_ the concept. For example, an `Array` concept can be defined in terms of requirements like this (there
will be more requirements):

- There must be a nested type called `Element`, defining the type of the elements
- There must be a member function called `rank`, returning the dimensionality as an integral
- There must be a member function called `extents`, returning a collection of extents in each dimension

Depending on the programming language, it may be possible to enforce concepts (e.g. in C++). In any case,
concepts can be documented, and used to describe types that can be passed into function, for example. That is
what these pages are intended for. Since LUE contains operations and types that can be used from multiple
programming languages, instead of documenting each language specific API, duplicating a lot of text, the LUE
documentation is written in terms of language agnostic operations, concepts, and types modelling these
concepts. Since examples are included, for the various supported programming languages, it is hoped that the
user can infer how to use LUE from a specific language.

Each page in this section describes a concept and the types modelling these concepts. In the reference pages
for operations, signatures are shown in terms of concepts. Additionally, information is provided about which
concrete types can be used.
