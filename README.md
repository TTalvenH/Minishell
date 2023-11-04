# Minishell
The Minishell project is a task to create a minimalistic Unix-like shell with the following features:
  -  Display a prompt when waiting for a new command.
   - Maintain a working command history.
   - Execute the correct executable based on the PATH variable or specified relative/absolute paths.
   - Avoid interpreting unclosed quotes or unnecessary special characters.
   - Handle single quotes ('), preventing interpretation of metacharacters within quoted sequences.
   - Handle double quotes (") except for $, preventing interpretation of metacharacters within quoted sequences.
   - Implement redirections: < (input), > (output), << (input until a delimiter is encountered), and >> (output in append mode).
   - Support pipes (|) to connect the output of one command to the input of the next.
   - Expand environment variables (e.g., $VAR) to their values.
   - Handle $? to expand to the exit status of the most recently executed foreground pipeline.
   - Handle ctrl-C (new prompt), ctrl-D (shell exit), and ctrl-\ (no action) in interactive mode.
   - Implement built-in commands: echo (-n option), cd (relative/absolute path), pwd, export, unset, env, and exit.
