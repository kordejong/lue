(reference-concept-field)=

# Field

```{mermaid}
flowchart TD
    Field --> Raster
    Field --> Array
    Field --> Scalar
    Field --> Value
```

A `Field` is a property of an object which is defined everywhere within the spatial bounds of the object. It
is a high level parent concept, with only a few requirements, related to general aspects that all child
concepts have in common. Field concepts are related to:

- Element type: it must be possible to query each field for its element type
