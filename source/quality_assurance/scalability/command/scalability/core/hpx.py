def thread_binding(nr_threads: int) -> str:
    """
    Return a string for binding `nr_threads` OS threads to the first processing unit of each core
    """
    return "thread:0-{}=core:0-{}.pu:0".format(nr_threads - 1, nr_threads - 1)


def performance_counter_name_to_property_name(counter_name: str) -> str:
    """
    Given an HPX performance `counter_name`, return a LUE property name

    This is the inverse of :py:func:`property_name_to_performance_counter_name`.

    - The `counter_name` must not contain forward slashes
    - Forward slashes are replaced by pipe symbols
    """
    assert counter_name.find("|") == -1, counter_name
    return counter_name.replace("/", "|")


def property_name_to_performance_counter_name(property_name: str) -> str:
    """
    Given a LUE `property_name`, return an HPX performance counter name

    This is the inverse of :py:func:`performance_counter_name_to_property_name`.

    - The `property_name` must not contain forward slashes
    - Pipe symbols are replaced by forward slashes
    """
    assert property_name.find("/") == -1, property_name
    return property_name.replace("|", "/")
