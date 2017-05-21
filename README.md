# Stream-Processing

In this work we intend to build a stream processing system. These systems use a network of components to filter, modify, and process an event flow. Typically, they allow for handling a large amount of data by exploiting concurrency between pipelining and between instances of the same component. An example of a stream processing system for distributed systems is Apache Storm2.

In this case, it is intended an implementation for Unix systems, exploring the similarity of stream processing with composite text filters in pipeline. Thus, it is assumed that each event is a line of text, with fields separated by a colon (:), its size being less than PIPE_BUF.

This work has two parts: the implementation of a set of components, which perform elementary tasks; And the implementation of the system that makes up and controls the processing network.
