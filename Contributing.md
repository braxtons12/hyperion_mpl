Feel free to submit issues, pull requests, etc.!
When contributing code, please follow the general style expectations of Hyperion projects:
    - Follow the project .clang-format (except in judicious cases of templates or requires clauses ruining formatting),
    - Use trailing returns types,
    - Prefer assign-init (e.g. auto var = value;) over direct-init (e.g. auto var{value}),
    - By default, prefer simplicity and correctness over performance
    - We try to target Clang/LLVM 15, which doesn't yet support usage of concepts in function
    definitions occurring after the declaration (for example, defining a function template outside
    of the class it was declared in). In these cases, revert to using `enable_if` instead.
