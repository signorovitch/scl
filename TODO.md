EXCEPTION HANDLING: exception ast type should have as data a giant enum of
possible types, rather than a char* message. A description of each type could be
handled under the exception type and print logic. For now, executor checks
message for special exceptions e.g. exit().

Change editor to GNU Readline.
Make variables persist through lines in the editor.

Return syntax errors as exceptions.
