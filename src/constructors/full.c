/* -*- mode: C -*-  */
/*
   IGraph library.
   Copyright (C) 2005-2021 The igraph development team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA

*/

#include "igraph_constructors.h"

#include "igraph_interface.h"

/**
 * \ingroup generators
 * \function igraph_full
 * \brief Creates a full graph (directed or undirected, with or without loops).
 *
 * </para><para>
 * In a full graph every possible edge is present, every vertex is
 * connected to every other vertex. A full graph in \c igraph should be
 * distinguished from the concept of complete graphs as used in graph theory.
 * If n is a positive integer, then the complete graph K_n on n vertices is
 * the undirected simple graph with the following property. For any distinct
 * pair (u,v) of vertices in K_n, uv (or equivalently vu) is an edge of K_n.
 * In \c igraph, a full graph on n vertices can be K_n, a directed version of
 * K_n, or K_n with at least one loop edge. In any case, if F is a full graph
 * on n vertices as generated by \c igraph, then K_n is a subgraph of the
 * undirected version of F.
 *
 * \param graph Pointer to an uninitialized graph object.
 * \param n Integer, the number of vertices in the graph.
 * \param directed Logical, whether to create a directed graph.
 * \param loops Logical, whether to include self-edges (loops).
 * \return Error code:
 *         \c IGRAPH_EINVAL: invalid number of vertices.
 *
 * Time complexity: O(|V|+|E|),
 * |V| is the number of vertices,
 * |E| the number of edges in the
 * graph. Of course this is the same as
 * O(|E|)=O(|V||V|)
 * here.
 *
 * \sa \ref igraph_lattice(), \ref igraph_star(), \ref igraph_tree()
 * for creating other regular structures.
 *
 * \example examples/simple/igraph_full.c
 */
igraph_error_t igraph_full(igraph_t *graph, igraph_integer_t n, igraph_bool_t directed,
                igraph_bool_t loops) {

    igraph_vector_t edges = IGRAPH_VECTOR_NULL;
    long int i, j;

    if (n < 0) {
        IGRAPH_ERROR("invalid number of vertices", IGRAPH_EINVAL);
    }

    IGRAPH_VECTOR_INIT_FINALLY(&edges, 0);

    if (directed && loops) {
        IGRAPH_CHECK(igraph_vector_reserve(&edges, n * n));
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                igraph_vector_push_back(&edges, i); /* reserved */
                igraph_vector_push_back(&edges, j); /* reserved */
            }
        }
    } else if (directed && !loops) {
        IGRAPH_CHECK(igraph_vector_reserve(&edges, n * (n - 1)));
        for (i = 0; i < n; i++) {
            for (j = 0; j < i; j++) {
                igraph_vector_push_back(&edges, i); /* reserved */
                igraph_vector_push_back(&edges, j); /* reserved */
            }
            for (j = i + 1; j < n; j++) {
                igraph_vector_push_back(&edges, i); /* reserved */
                igraph_vector_push_back(&edges, j); /* reserved */
            }
        }
    } else if (!directed && loops) {
        IGRAPH_CHECK(igraph_vector_reserve(&edges, n * (n + 1) / 2));
        for (i = 0; i < n; i++) {
            for (j = i; j < n; j++) {
                igraph_vector_push_back(&edges, i); /* reserved */
                igraph_vector_push_back(&edges, j); /* reserved */
            }
        }
    } else {
        IGRAPH_CHECK(igraph_vector_reserve(&edges, n * (n - 1) / 2));
        for (i = 0; i < n; i++) {
            for (j = i + 1; j < n; j++) {
                igraph_vector_push_back(&edges, i); /* reserved */
                igraph_vector_push_back(&edges, j); /* reserved */
            }
        }
    }

    IGRAPH_CHECK(igraph_create(graph, &edges, n, directed));
    igraph_vector_destroy(&edges);
    IGRAPH_FINALLY_CLEAN(1);

    return 0;
}

/**
 * \function igraph_full_citation
 * Creates a full citation graph
 *
 * This is a directed graph, where every <code>i->j</code> edge is
 * present if and only if <code>j&lt;i</code>.
 * If the \c directed argument is zero then an undirected graph is
 * created, and it is just a full graph.
 * \param graph Pointer to an uninitialized graph object, the result
 *    is stored here.
 * \param n The number of vertices.
 * \param directed Whether to created a directed graph. If zero an
 *    undirected graph is created.
 * \return Error code.
 *
 * Time complexity: O(|V|^2), as we have many edges.
 */
igraph_error_t igraph_full_citation(igraph_t *graph, igraph_integer_t n,
                         igraph_bool_t directed) {
    igraph_vector_t edges;
    long int i, j, ptr = 0;

    IGRAPH_VECTOR_INIT_FINALLY(&edges, n * (n - 1));
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            VECTOR(edges)[ptr++] = i;
            VECTOR(edges)[ptr++] = j;
        }
    }

    IGRAPH_CHECK(igraph_create(graph, &edges, n, directed));
    igraph_vector_destroy(&edges);
    IGRAPH_FINALLY_CLEAN(1);
    return 0;
}
