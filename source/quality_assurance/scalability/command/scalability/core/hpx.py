def thread_binding(nr_threads: int) -> str:
    # Bind OS threads to the first processing unit of each core
    return "thread:0-{}=core:0-{}.pu:0".format(nr_threads - 1, nr_threads - 1)


def performance_counter_name_to_property_name(counter_name: str) -> str:
    assert counter_name.find("|") == -1, counter_name
    return counter_name.replace("/", "|")


def property_name_to_performance_counter_name(property_name: str) -> str:
    assert property_name.find("/") == -1, property_name
    return property_name.replace("|", "/")
