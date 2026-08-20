(lue-translate)=

# lue_translate

The lue_translate command can be used to import data into a LUE dataset or export data from a LUE dataset. For
this the command supports two subcommands: `import` and `export`.

For information about the command's usage, type `lue_translate --help`{l=bash} on the command-line.

## Import

For information about the subcommand's usage, type `lue_translate import --help`{l=bash} on the command-line.

### GDAL raster

TODO

### GDAL raster stack

TODO

### GDAL feature

TODO

### JSON

#### Raster

Example:

```json
{

}
```

## Export

For information about the subcommand's usage, type `lue_translate export --help`{l=bash} on the command-line.

### DOT

To get an impression of the layout of a LUE dataset, a [Graphviz](http://www.graphviz.org) graph in DOT
format can be generated and written to a file. The `dot` tool can then be used to generate yet another file,
to be used for visualization. This can be a file formatted as SVG, PDF, or PNG, for example.

```bash
lue_translate export --meta dot_properties.json my_dataset.lue my_dataset.dot
```

The conversion process can be tweaked using the metadata file. This file can contains three sections:

| Group | Role                           |
| ----- | ------------------------------ |
| dot   | Configuration of the DOT graph |
| hdf5  | Formatting HDF5 objects        |
| lue   | Formatting LUE objects         |

The following listing shows an example.

```json
{
    "dot": {
        "node": {
            "fontcolor": "white"
        }
    },

    "hdf5": {
        "dataset": {
            "shape": "oval"
        },
        "dataset": {
            "shape": "box"
        }
    },

    "lue": {
        "phenomenon": {
            "fillcolor": "#791d72"
        },
        "property_set": {
            "fillcolor": "#9a0000"
        },
        "property": {
            "fillcolor": "#419702"
        },
        "domain": {
            "fillcolor": "#094d8e"
        },
        "space_domain": {
            "fillcolor": "#36a2c9"
        },
        "value": {
            "fillcolor": "#f08000",
            "show_details": false
        }
    }
}
```

This command can be used to convert a file containing a graph to an SVG file for visualization:

```bash
dot -Tsvg -o my_dataset.svg my_dataset.dot
```

### Shapefile

A single domain in a LUE dataset can be translated to a Shapefile:

```bash
lue_translate export --meta meta.json my_dataset.lue my_dataset.shp
```

The domain-items end up as geometries in the Shapefile. Which domain to translate is determined by the
contents of the metadata file. In the following example, (the domain of) `my_property_set` is selected.

```json
{
    "my_dataset": {
        "phenomena": [
            {
                "name": "my_phenomenon",
                "property_sets": [
                    {
                        "name": "my_property_set"
                    }
                ]
            }
        ]
    }
}
```
