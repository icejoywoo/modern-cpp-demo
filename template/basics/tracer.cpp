//
// Created by icejoywoo on 2022/7/18.
//

#include "tracer.hpp"

long SortTracer::n_created = 0;
long SortTracer::n_destroyed = 0;
long SortTracer::n_assigned = 0;
long SortTracer::n_compared = 0;
long SortTracer::n_max_live = 0;