#include <random>
#define BOOST_TEST_MODULE lue netcdf4 use_case weather_forecast
#include "lue/netcdf4.hpp"
#include <boost/test/included/unit_test.hpp>

// Use_case from:
// https://notebook.community/julienchastang/unidata-python-workshop/notebooks/CF%20Conventions/NetCDF%20and%20CF%20-%20The%20Basics
// TODO: This is an example of using netcdf4 API to create a dataset which is organized according to the CF
// conventions. Rename / move file to reflect this.
// TODO: Add a similar use_case test to the cf API tests


BOOST_AUTO_TEST_CASE(gridded_data)
{

    // - Forecast of three dimensional temperature at several times
    // - Native coordinate system of the model is on a regular grid that represents the Earth on a Lambert
    //   conformal projection.

    // // Twelve hours of hourly output starting at 22Z today
    // start = datetime.utcnow().replace(hour=22, minute=0, second=0, microsecond=0)
    // times = np.array([start + timedelta(hours=h) for h in range(13)])
    //
    // // 3km spacing in x and y
    // x = np.arange(-150, 153, 3)
    // y = np.arange(-100, 100, 3)
    //
    // // Standard pressure levels in hPa
    // press = np.array([1000, 925, 850, 700, 500, 300, 250])
    //
    // temps = np.random.randn(times.size, press.size, y.size, x.size)

    std::size_t const nr_time_points{13};
    std::size_t const nr_x_coordinates{101};
    std::size_t const nr_y_coordinates{67};

    std::vector<std::int32_t> time_points(nr_time_points);
    std::vector<float> x_coordinates(nr_x_coordinates);
    std::vector<float> y_coordinates(nr_y_coordinates);

    // counts since an epoch
    std::ranges::iota(time_points, 0);

    std::ranges::generate(
        x_coordinates,
        []() -> float
        {
            static float value{-150};
            float const result = value;
            value += 3;
            return result;
        });
    std::ranges::generate(
        y_coordinates,
        []() -> float
        {
            static float value{-100};
            float const result = value;
            value += 3;
            return result;
        });

    std::vector<float> pressure_levels{1000, 925, 850, 700, 500, 300, 250};
    std::size_t const nr_pressure_levels{pressure_levels.size()};

    // temps = np.random.randn(times.size, press.size, y.size, x.size)

    std::vector<float> temps(nr_time_points * nr_pressure_levels * nr_y_coordinates * nr_x_coordinates);

    auto draw_temp = []() -> auto
    {
        static std::random_device random_device{};
        static std::mt19937 random_number_engine{random_device()};
        static std::normal_distribution<float> distribution{0.0, 1.0};

        return distribution(random_number_engine);
    };

    std::ranges::generate(temps, draw_temp);

    // nc = Dataset('forecast_model.nc', 'w', format='NETCDF4_CLASSIC', diskless=True)
    std::string const dataset_name = "forecast_model.nc";

    {
        // Initialize layout of dataset
        auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);

        // 1. Add recommended global attribute data

        // nc.Conventions = 'CF-1.7'
        // nc.title = 'Forecast model run'
        // nc.institution = 'Unidata'
        // nc.source = 'WRF-1.5'
        // nc.history = str(datetime.utcnow()) + ' Python'
        // nc.references = ''
        // nc.comment = ''

        BOOST_REQUIRE(!dataset.has_attribute("Conventions"));
        dataset.set_conventions({"CF-1.7"});

        BOOST_REQUIRE(!dataset.has_attribute("title"));
        dataset.add_attribute("title", "Forecast model run");

        BOOST_REQUIRE(!dataset.has_attribute("institution"));
        dataset.add_attribute("institution", "Unidata");

        BOOST_REQUIRE(!dataset.has_attribute("source"));
        dataset.add_attribute("source", "WRF-1.5");

        BOOST_REQUIRE(!dataset.has_attribute("history"));
        dataset.add_attribute("history", "20251210 C++");

        BOOST_REQUIRE(!dataset.has_attribute("references"));
        dataset.add_attribute("references", "");

        BOOST_REQUIRE(!dataset.has_attribute("comment"));
        dataset.add_attribute("comment", "");

        // 2. Define dimensions

        // nc.createDimension('forecast_time', None)
        // nc.createDimension('x', x.size)
        // nc.createDimension('y', y.size)
        // nc.createDimension('pressure', press.size)

        dataset.add_dimension("forecast_time");
        dataset.add_dimension("x", nr_x_coordinates);
        dataset.add_dimension("y", nr_y_coordinates);
        dataset.add_dimension("pressure", nr_pressure_levels);
    }

    {
        // Test layout of dataset
        auto dataset = lue::netcdf4::Dataset::open(dataset_name);

        std::vector<std::string> const conventions_we_want{"CF-1.7"};
        std::vector<std::string> const conventions_we_got = dataset.conventions();
        BOOST_CHECK_EQUAL_COLLECTIONS(
            conventions_we_got.begin(),
            conventions_we_got.end(),
            conventions_we_want.begin(),
            conventions_we_want.end());

        BOOST_CHECK_EQUAL(dataset.attribute("title").value(), "Forecast model run");
        BOOST_CHECK_EQUAL(dataset.attribute("institution").value(), "Unidata");
        BOOST_CHECK_EQUAL(dataset.attribute("source").value(), "WRF-1.5");
        BOOST_CHECK_EQUAL(dataset.attribute("history").value(), "20251210 C++");
        BOOST_CHECK_EQUAL(dataset.attribute("references").value(), "");
        BOOST_CHECK_EQUAL(dataset.attribute("comment").value(), "");

        auto const forecast_time_dimension = dataset.dimension("forecast_time");
        BOOST_CHECK_EQUAL(forecast_time_dimension.name(), "forecast_time");
        BOOST_CHECK_EQUAL(forecast_time_dimension.length(), 0);  // Number of records written so far

        auto const x_dimension = dataset.dimension("x");
        BOOST_CHECK_EQUAL(x_dimension.name(), "x");
        BOOST_CHECK_EQUAL(x_dimension.length(), nr_x_coordinates);

        auto const y_dimension = dataset.dimension("y");
        BOOST_CHECK_EQUAL(y_dimension.name(), "y");
        BOOST_CHECK_EQUAL(y_dimension.length(), nr_y_coordinates);

        auto const pressure_dimension = dataset.dimension("pressure");
        BOOST_CHECK_EQUAL(pressure_dimension.name(), "pressure");
        BOOST_CHECK_EQUAL(pressure_dimension.length(), nr_pressure_levels);
    }

    {
        // Add data
        auto dataset = lue::netcdf4::Dataset::open(dataset_name, NC_WRITE);

        // 3. Create and fill variables

        // temps_var = nc.createVariable('Temperature', datatype=np.float32,
        //     dimensions=('forecast_time', 'pressure', 'y', 'x'),
        //     zlib=True)
        // temps_var[:] = temps

        // TODO: zlib

        lue::netcdf4::Variable temp_var = dataset.add_variable(
            "Temperature",
            NC_FLOAT,
            {dataset.dimension("forecast_time"),
             dataset.dimension("pressure"),
             dataset.dimension("y"),
             dataset.dimension("x")});

        // NOTE: Alternative: write data in a loop, passing in an updated hyperslab each time
        lue::netcdf4::Hyperslab const hyperslab{
            {0, 0, 0, 0}, {nr_time_points, nr_pressure_levels, nr_y_coordinates, nr_x_coordinates}};
        temp_var.write(hyperslab, temps.data());

        // temps_var.units = 'Kelvin'
        // temps_var.standard_name = 'air_temperature'
        // temps_var.long_name = 'Forecast air temperature'
        // temps_var.missing_value = -9999
        // temps_var

        temp_var.add_attribute("units", "Kelvin");
        temp_var.add_attribute("standard_name", "air_temperature");
        temp_var.add_attribute("long_name", "Forecast air temperature");
        temp_var.add_attribute<float>("missing_value", -9999);

        // Coordinate variables

        // x_var = nc.createVariable('x', np.float32, ('x',))
        // x_var[:] = x
        // x_var.units = 'km'
        // x_var.axis = 'X' # Optional
        // x_var.standard_name = 'projection_x_coordinate'
        // x_var.long_name = 'x-coordinate in projected coordinate system'
        //
        // y_var = nc.createVariable('y', np.float32, ('y',))
        // y_var[:] = y
        // y_var.units = 'km'
        // y_var.axis = 'Y' # Optional
        // y_var.standard_name = 'projection_y_coordinate'
        // y_var.long_name = 'y-coordinate in projected coordinate system'

        lue::netcdf4::Variable x_var = dataset.add_variable("x", NC_FLOAT, {dataset.dimension("x")});
        x_var.write(x_coordinates.data());
        x_var.add_attribute("units", "km");
        x_var.add_attribute("axis", "X");
        x_var.add_attribute("standard_name", "projection_x_coordinate");
        x_var.add_attribute("long_name", "x-coordinate in projected coordinate system");

        lue::netcdf4::Variable y_var = dataset.add_variable("y", NC_FLOAT, {dataset.dimension("y")});
        y_var.write(y_coordinates.data());
        y_var.add_attribute("units", "km");
        y_var.add_attribute("axis", "Y");
        y_var.add_attribute("standard_name", "projection_y_coordinate");
        y_var.add_attribute("long_name", "y-coordinate in projected coordinate system");

        // press_var = nc.createVariable('pressure', np.float32, ('pressure',))
        // press_var[:] = press
        // press_var.units = 'hPa'
        // press_var.axis = 'Z'  # Optional
        // press_var.standard_name = 'air_pressure'
        // press_var.positive = 'down'  # Optional

        lue::netcdf4::Variable press_var =
            dataset.add_variable("pressure", NC_FLOAT, {dataset.dimension("pressure")});
        press_var.write(pressure_levels.data());
        press_var.add_attribute("units", "hPa");
        press_var.add_attribute("axis", "Z");
        press_var.add_attribute("standard_name", "air_pressure");
        press_var.add_attribute("positive", "down");

        // from cftime import date2num
        // time_units = 'hours since {:%Y-%m-%d 00:00}'.format(times[0])
        // time_vals = date2num(times, time_units)
        //
        // time_var = nc.createVariable('forecast_time', np.int32, ('forecast_time',))
        // time_var[:] = time_vals
        // time_var.units = time_units
        // time_var.axis = 'T'  # Optional
        // time_var.standard_name = 'time'  # Optional
        // time_var.long_name = 'time'

        lue::netcdf4::Variable time_var =
            dataset.add_variable("forecast_time", NC_INT, {dataset.dimension("forecast_time")});
        time_var.write(time_points.data());
        time_var.add_attribute("units", "hours since 2025-12-10 12:00");
        time_var.add_attribute("axis", "T");
        time_var.add_attribute("standard_name", "time");
        time_var.add_attribute("long_name", "time");

        // Auxiliary variables

        // from pyproj import Proj
        // X, Y = np.meshgrid(x, y)
        // lcc = Proj({'proj':'lcc', 'lon_0':-105, 'lat_0':40, 'a':6371000.,
        //             'lat_1':25})
        // lon, lat = lcc(X * 1000, Y * 1000, inverse=True)

        // TODO: Translate coordinates using proj

        auto const lon = x_coordinates;
        auto const lat = y_coordinates;

        // lon_var = nc.createVariable('lon', np.float64, ('y', 'x'))
        // lon_var[:] = lon
        // lon_var.units = 'degrees_east'
        // lon_var.standard_name = 'longitude'  # Optional
        // lon_var.long_name = 'longitude'
        //
        // lat_var = nc.createVariable('lat', np.float64, ('y', 'x'))
        // lat_var[:] = lat
        // lat_var.units = 'degrees_north'
        // lat_var.standard_name = 'latitude'  # Optional
        // lat_var.long_name = 'latitude'

        lue::netcdf4::Variable lon_var =
            dataset.add_variable("lon", NC_DOUBLE, {dataset.dimension("y"), dataset.dimension("x")});
        lon_var.write(lon.data());
        lon_var.add_attribute("units", "degrees_east");
        lon_var.add_attribute("standard_name", "longitude");
        lon_var.add_attribute("long_name", "longitude");

        lue::netcdf4::Variable lat_var =
            dataset.add_variable("lat", NC_DOUBLE, {dataset.dimension("y"), dataset.dimension("x")});
        lat_var.write(lat.data());
        lat_var.add_attribute("units", "degrees_north");
        lat_var.add_attribute("standard_name", "latitude");
        lat_var.add_attribute("long_name", "latitude");

        // TODO: reorganize things? First set up dimensions, coordinate, and auxiliary variables, then data
        //       variables.
        temp_var.add_attribute("coordinates", "lon lat");

        // Coordinate system information

        // Grid mapping variable: dummy scalar variable as a namespace for holding all required information.
        // Relevant variables can reference it with their grid_mapping attribute.

        // proj_var = nc.createVariable('lambert_projection', np.int32, ())
        // proj_var.grid_mapping_name = 'lambert_conformal_conic'
        // proj_var.standard_parallel = 25.
        // proj_var.latitude_of_projection_origin = 40.
        // proj_var.longitude_of_central_meridian = -105.
        // proj_var.semi_major_axis = 6371000.0

        lue::netcdf4::Variable proj_var = dataset.add_variable("lambert_projection", NC_INT);
        proj_var.add_attribute("grid_mapping_name", "lambert_conformal_conic");
        proj_var.add_attribute("standard_parallel", double{25});
        proj_var.add_attribute("latitude_of_projection_origin", double{40});
        proj_var.add_attribute("longitude_of_central_meridian", double{-105});
        proj_var.add_attribute("semi_major_axis", double{6371000});

        temp_var.add_attribute("grid_mapping", proj_var.name());

        // TODO: Cell bounds. How does this work?
    }

    {
        // Test data
        auto dataset = lue::netcdf4::Dataset::open(dataset_name);

        {
            BOOST_REQUIRE(dataset.has_variable("Temperature"));
            lue::netcdf4::Variable const temp_var = dataset.variable("Temperature");

            BOOST_CHECK_EQUAL(temp_var.name(), "Temperature");
            BOOST_CHECK_EQUAL(temp_var.type(), NC_FLOAT);

            BOOST_CHECK_EQUAL(temp_var.nr_dimensions(), 4);
            std::vector<lue::netcdf4::Dimension> const dimensions{temp_var.dimensions()};
            BOOST_CHECK_EQUAL(dimensions.size(), 4);
            BOOST_CHECK_EQUAL(dimensions[0].name(), "forecast_time");
            BOOST_CHECK_EQUAL(dimensions[0].length(), nr_time_points);  // Expanded
            BOOST_CHECK_EQUAL(dimensions[1].name(), "pressure");
            BOOST_CHECK_EQUAL(dimensions[1].length(), nr_pressure_levels);
            BOOST_CHECK_EQUAL(dimensions[2].name(), "y");
            BOOST_CHECK_EQUAL(dimensions[2].length(), nr_y_coordinates);
            BOOST_CHECK_EQUAL(dimensions[3].name(), "x");
            BOOST_CHECK_EQUAL(dimensions[3].length(), nr_x_coordinates);

            BOOST_CHECK_EQUAL(temp_var.attribute("units").value(), "Kelvin");
            BOOST_CHECK_EQUAL(temp_var.attribute("standard_name").value(), "air_temperature");
            BOOST_CHECK_EQUAL(temp_var.attribute("long_name").value(), "Forecast air temperature");
            BOOST_CHECK_EQUAL(temp_var.attribute("missing_value").value<float>(), -9999);
            BOOST_CHECK_EQUAL(temp_var.attribute("coordinates").value(), "lon lat");
            BOOST_CHECK_EQUAL(temp_var.attribute("grid_mapping").value(), "lambert_projection");
        }

        {
            BOOST_REQUIRE(dataset.has_variable("x"));
            lue::netcdf4::Variable const x_var = dataset.variable("x");

            BOOST_CHECK_EQUAL(x_var.nr_dimensions(), 1);
            std::vector<lue::netcdf4::Dimension> const dimensions{x_var.dimensions()};
            BOOST_CHECK_EQUAL(dimensions.size(), 1);
            BOOST_CHECK_EQUAL(dimensions[0].name(), "x");
            BOOST_CHECK_EQUAL(dimensions[0].length(), nr_x_coordinates);

            BOOST_CHECK_EQUAL(x_var.attribute("units").value(), "km");
            BOOST_CHECK_EQUAL(x_var.attribute("axis").value(), "X");
            BOOST_CHECK_EQUAL(x_var.attribute("standard_name").value(), "projection_x_coordinate");
            BOOST_CHECK_EQUAL(
                x_var.attribute("long_name").value(), "x-coordinate in projected coordinate system");
        }

        {
            BOOST_REQUIRE(dataset.has_variable("y"));
            lue::netcdf4::Variable const y_var = dataset.variable("y");

            BOOST_CHECK_EQUAL(y_var.nr_dimensions(), 1);
            std::vector<lue::netcdf4::Dimension> const dimensions{y_var.dimensions()};
            BOOST_CHECK_EQUAL(dimensions.size(), 1);
            BOOST_CHECK_EQUAL(dimensions[0].name(), "y");
            BOOST_CHECK_EQUAL(dimensions[0].length(), nr_y_coordinates);

            BOOST_CHECK_EQUAL(y_var.attribute("units").value(), "km");
            BOOST_CHECK_EQUAL(y_var.attribute("axis").value(), "Y");
            BOOST_CHECK_EQUAL(y_var.attribute("standard_name").value(), "projection_y_coordinate");
            BOOST_CHECK_EQUAL(
                y_var.attribute("long_name").value(), "y-coordinate in projected coordinate system");
        }

        {
            BOOST_REQUIRE(dataset.has_variable("pressure"));
            lue::netcdf4::Variable const pressure_var = dataset.variable("pressure");

            BOOST_CHECK_EQUAL(pressure_var.nr_dimensions(), 1);
            std::vector<lue::netcdf4::Dimension> const dimensions{pressure_var.dimensions()};
            BOOST_CHECK_EQUAL(dimensions.size(), 1);
            BOOST_CHECK_EQUAL(dimensions[0].name(), "pressure");
            BOOST_CHECK_EQUAL(dimensions[0].length(), nr_pressure_levels);

            BOOST_CHECK_EQUAL(pressure_var.attribute("units").value(), "hPa");
            BOOST_CHECK_EQUAL(pressure_var.attribute("axis").value(), "Z");
            BOOST_CHECK_EQUAL(pressure_var.attribute("standard_name").value(), "air_pressure");
            BOOST_CHECK_EQUAL(pressure_var.attribute("positive").value(), "down");
        }

        {
            BOOST_REQUIRE(dataset.has_variable("forecast_time"));
            lue::netcdf4::Variable const forecast_time = dataset.variable("forecast_time");

            BOOST_CHECK_EQUAL(forecast_time.nr_dimensions(), 1);
            std::vector<lue::netcdf4::Dimension> const dimensions{forecast_time.dimensions()};
            BOOST_CHECK_EQUAL(dimensions.size(), 1);
            BOOST_CHECK_EQUAL(dimensions[0].name(), "forecast_time");
            BOOST_CHECK_EQUAL(dimensions[0].length(), nr_time_points);

            BOOST_CHECK_EQUAL(forecast_time.attribute("units").value(), "hours since 2025-12-10 12:00");
            BOOST_CHECK_EQUAL(forecast_time.attribute("axis").value(), "T");
            BOOST_CHECK_EQUAL(forecast_time.attribute("standard_name").value(), "time");
            BOOST_CHECK_EQUAL(forecast_time.attribute("long_name").value(), "time");
        }

        {
            BOOST_REQUIRE(dataset.has_variable("lon"));
            lue::netcdf4::Variable const lon = dataset.variable("lon");

            BOOST_CHECK_EQUAL(lon.nr_dimensions(), 2);
            std::vector<lue::netcdf4::Dimension> const dimensions{lon.dimensions()};
            BOOST_CHECK_EQUAL(dimensions.size(), 2);
            BOOST_CHECK_EQUAL(dimensions[0].name(), "y");
            BOOST_CHECK_EQUAL(dimensions[0].length(), nr_y_coordinates);
            BOOST_CHECK_EQUAL(dimensions[1].name(), "x");
            BOOST_CHECK_EQUAL(dimensions[1].length(), nr_x_coordinates);

            BOOST_CHECK_EQUAL(lon.attribute("units").value(), "degrees_east");
            BOOST_CHECK_EQUAL(lon.attribute("standard_name").value(), "longitude");
            BOOST_CHECK_EQUAL(lon.attribute("long_name").value(), "longitude");
        }

        {
            BOOST_REQUIRE(dataset.has_variable("lat"));
            lue::netcdf4::Variable const lat = dataset.variable("lat");

            BOOST_CHECK_EQUAL(lat.nr_dimensions(), 2);
            std::vector<lue::netcdf4::Dimension> const dimensions{lat.dimensions()};
            BOOST_CHECK_EQUAL(dimensions.size(), 2);
            BOOST_CHECK_EQUAL(dimensions[0].name(), "y");
            BOOST_CHECK_EQUAL(dimensions[0].length(), nr_y_coordinates);
            BOOST_CHECK_EQUAL(dimensions[1].name(), "x");
            BOOST_CHECK_EQUAL(dimensions[1].length(), nr_x_coordinates);

            BOOST_CHECK_EQUAL(lat.attribute("units").value(), "degrees_north");
            BOOST_CHECK_EQUAL(lat.attribute("standard_name").value(), "latitude");
            BOOST_CHECK_EQUAL(lat.attribute("long_name").value(), "latitude");
        }

        {
            BOOST_REQUIRE(dataset.has_variable("lambert_projection"));
            lue::netcdf4::Variable const proj_var = dataset.variable("lambert_projection");

            BOOST_CHECK_EQUAL(proj_var.nr_dimensions(), 0);

            BOOST_CHECK_EQUAL(proj_var.attribute("grid_mapping_name").value(), "lambert_conformal_conic");
            BOOST_CHECK_EQUAL(proj_var.attribute("standard_parallel").value<double>(), 25);
            BOOST_CHECK_EQUAL(proj_var.attribute("latitude_of_projection_origin").value<double>(), 40);
            BOOST_CHECK_EQUAL(proj_var.attribute("longitude_of_central_meridian").value<double>(), -105);
            BOOST_CHECK_EQUAL(proj_var.attribute("semi_major_axis").value<double>(), 6371000);
        }
    }
}


BOOST_AUTO_TEST_CASE(observational_data)
{
    // The CF conventions describe this as conventions for Discrete Sampling Geometeries (DSG)

    // Here: data that are regularly sampled (say, all at the same heights)
    // Define some sample profile data, all at a few heights less than 1000m

    // lons = np.array([-97.1, -105, -80])
    // lats = np.array([35.25, 40, 27])
    // heights = np.linspace(10, 1000, 10)
    // temps = np.random.randn(lats.size, heights.size)
    // stids = ['KBOU', 'KOUN', 'KJUP']

    std::vector<float> lons = {-97.1, -105, -80};
    std::vector<float> lats = {35.25, 40, 27};
    std::vector<float> heights(100);
    std::ranges::generate(
        heights,
        []() -> float
        {
            static float value{10};
            float const result = value;
            value += 10;
            return result;
        });
    std::vector<float> temps(lats.size() * heights.size());
    std::ranges::generate(
        temps,
        []() -> auto
        {
            static std::random_device random_device{};
            static std::mt19937 random_number_engine{random_device()};
            static std::normal_distribution<float> distribution{0.0, 1.0};

            return distribution(random_number_engine);
        });
    std::vector<std::array<char, 5>> stids{{"KBOU"}, {"KOUN"}, {"KJUP"}};

    std::string const dataset_name = "obs_data.nc";

    {
        // Since this is profile data, heights is one dimension. Station is another dimension.
        // Feature type is profile, to indicate that file holds "an ordered set of data points along a
        // vertical line at a fixed horizontal position and fixed time". Another dimension is used for storing
        // station ID strings.

        // nc = Dataset('obs_data.nc', 'w', format='NETCDF4_CLASSIC', diskless=True)
        // nc.createDimension('station', lats.size)
        // nc.createDimension('heights', heights.size)
        // nc.createDimension('str_len', 4)
        // nc.Conventions = 'CF-1.7'
        // nc.featureType = 'profile'

        auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);

        dataset.add_dimension("station", lats.size());
        dataset.add_dimension("heights", heights.size());
        dataset.add_dimension("str_len", 4);
        dataset.set_conventions({"CF-1.7"});
        dataset.add_attribute("featureType", "profile");

        // instance variables, referring to an instance of a feature

        // lon_var = nc.createVariable('lon', np.float64, ('station',))
        // lon_var.units = 'degrees_east'
        // lon_var.standard_name = 'longitude'
        //
        // lat_var = nc.createVariable('lat', np.float64, ('station',))
        // lat_var.units = 'degrees_north'
        // lat_var.standard_name = 'latitude'

        lue::netcdf4::Variable lon_var =
            dataset.add_variable("lon", NC_DOUBLE, {dataset.dimension("station")});
        lon_var.add_attribute("units", "degrees_east");
        lon_var.add_attribute("standard_name", "longitude");

        lue::netcdf4::Variable lat_var =
            dataset.add_variable("lat", NC_DOUBLE, {dataset.dimension("station")});
        lat_var.add_attribute("units", "degrees_north");
        lat_var.add_attribute("standard_name", "latitude");

        // height coordinate variable

        // heights_var = nc.createVariable('heights', np.float32, ('heights',))
        // heights_var.units = 'meters'
        // heights_var.standard_name = 'altitude'
        // heights_var.positive = 'up'
        // heights_var[:] = heights

        lue::netcdf4::Variable heights_var =
            dataset.add_variable("heights", NC_FLOAT, {dataset.dimension("heights")});
        heights_var.add_attribute("units", "meters");
        heights_var.add_attribute("standard_name", "altitude");
        heights_var.add_attribute("positive", "up");
        heights_var.write(heights.data());

        // Station IDs variable
        // 2D variable, but one of the dimensions is simply there to facilitate treating strings as char
        // arrays.

        // stid_var = nc.createVariable('stid', 'c', ('station', 'str_len'))
        // stid_var.cf_role = 'profile_id'
        // stid_var.long_name = 'Station identifier'
        // stid_var[:] = stids

        lue::netcdf4::Variable stid_var = dataset.add_variable(
            "stid", NC_CHAR, {dataset.dimension("station"), dataset.dimension("str_len")});
        stid_var.add_attribute("cf_role", "profile_id");
        stid_var.add_attribute("long_name", "Station identifier");

        std::vector<std::array<char, 5>> stids{{"KBOU"}, {"KOUN"}, {"KJUP"}};

        for (std::size_t idx = 0; idx < stids.size(); ++idx)
        {
            lue::netcdf4::Hyperslab const hyperslab{{idx, 0}, {1, 4}};
            stid_var.write(hyperslab, stids[idx].data());
        }

        // Write profile data

        // time_var = nc.createVariable('time', np.float32, ())
        // time_var.units = 'minutes since 2019-07-16 17:00'
        // time_var.standard_name = 'time'
        // time_var[:] = [5.]
        //
        // temp_var = nc.createVariable('temperature', np.float32, ('station', 'heights'))
        // temp_var.units = 'celsius'
        // temp_var.standard_name = 'air_temperature'
        // temp_var.coordinates = 'lon lat heights time'

        lue::netcdf4::Variable time_var = dataset.add_variable("time", NC_FLOAT, {});
        time_var.add_attribute("units", "minutes since 2019-07-16 17:00");
        time_var.add_attribute("standard_name", "time");
        time_var.write(float{5});

        lue::netcdf4::Variable temp_var = dataset.add_variable(
            "temperature", NC_FLOAT, {dataset.dimension("station"), dataset.dimension("heights")});
        temp_var.add_attribute("units", "celsius");
        temp_var.add_attribute("standard_name", "air_temperature");
        temp_var.add_attribute("coordinates", "lon lat heights time");
        temp_var.write(temps.data());
    }

    {
        auto dataset = lue::netcdf4::Dataset::open(dataset_name);

        // TODO: test
    }
}
