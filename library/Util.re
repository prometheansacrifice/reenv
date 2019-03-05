let rec readUntilEndOfFile = (~previousLines=[], in_channel) =>
  try (
    {
      let row = input_line(in_channel);
      let lines =
        switch (row) {
        | "" => previousLines
        | r when String.unsafe_get(r, 0) == '#' => previousLines
        | _ => [row, ...previousLines]
        };
      readUntilEndOfFile(~previousLines=lines, in_channel);
    }
  ) {
  | End_of_file => List.rev(previousLines)
  };

let trimCitation = str => {
  let str = Str.global_replace(Str.regexp("'"), "\"", str);
  if (str == "") {
    str;
  } else if (str.[0] == '"') {
    // If we find a " in the start, assume it's one in the end as well
    let len = String.length(str);
    let until = Sys.win32 ? len - 3 : len - 2;

    String.sub(str, 1, until);
  } else {
    str;
  };
};

let replaceNewline = str => {
  Str.global_replace(Str.regexp("\\\\n"), "\n", str);
};

let escapeEquals = line =>
  switch (line) {
  | [name, ...value] =>
    let fixedValue =
      value
      |> String.concat("=")
      |> trimCitation
      |> replaceNewline
      |> String.trim;
    name ++ "=" ++ fixedValue;
  | [] => String.concat("", line) // This should never happen
  };
