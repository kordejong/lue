#pragma once

/*
https://www.youtube.com/watch?v=a5QvdSffqrM

- Requirements:
    - Write netcdf file
    - Using CF conventions (rename to to_netcdf_cf?)
    - Using .. conventions
    - Compliant with FAIR principles
    - Support adding to an existing netcdf file

- Layout the file:
    - Global attributes:
        - title
        - id
        - naming_authority
        - summary
        - creator_type
        - creator_name, semicolon sep
        - creator_email
        - creator_institution
        - creator_url
        - time_coverage_start
        - time_coverage_end
        - keywords
        - keywords_vocabulary
        - institution
        - publisher_name
        - publisher_email
        - publisher_url
        - license
        - Conventions, comma sep
        - project

        - geospatial_lat_min
        - geospatial_lat_max
        - geospatial_lon_min
        - geospatial_lon_max

        - date_created
        - history

    - Variable attributes:
        - units:
            - standardized
            - Time must be in ISO 8601 format. Make this an argument?
        - standard_name (standardized, from vocabulary). Make this an argument?
        - long_name
        - coverage_content_type:
            - standardized
            - easy for coordinates
            - make an argument for the data itself
        - _FillValue

- encoding per variable

Maybe pass this as an (optional?) argument: attributes
At the call site:
    attributes["some_global_attribute"] = "some value"
    attributes["some_variable"]["some_variable_attribute"] = "some value"
    to_lue(array, attributes)

- CF conventions
- ACDD convention

- Write netcdf file:
    - Write dimensions, if required
    - Write coordinate variables, if required
    - Write data variables. These should not already exist.

Compliance checker:
- IOOS compliance checker
    - https://github.com/ioos/compliance-checker/wiki/Installation
*/
