# c2python

> Convert your C code to Python using a C++ program

```cpp
#define STUDENT "Gabriel de Paula"
#define SUBJECT "Compiladores"
#define PROFESSOR "Flávio Schiavoni"
#define SEMESTER "2025/1"
```

&nbsp;

## How to use

Generate the executable:

```bash
make
```

Run the executable

```bash
./bin/compiler <source_file>
```

&nbsp;

## Simplified C Syntax

### 1. Identifiers

- Must start with an alphabetic character or an underscore (`_`).
- Can contain any combination of letters, digits, and underscores after the first character.

### 2. Numbers

- Must start with a digit.
- Can contain **one** dot `.` to separate the integer and decimal parts.
- May end with the letter `f` to represent a `float`.

### 3. String Literals

- Must start and end with a double quote `"`.
- The sequence `\"` is used to escape a double quote within the string.

### 4. Operators

- Supported operators include:  
  `*`, `/`, `%`, `+`, `-`, `==`, `!=`, `&&`, `||`, `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&`, `|`, `.`, `>`, `<`, `>=`, `<=`, `!`
- If an operator consists of multiple characters, each individual character must also be a valid operator on its own.

### 5. Separators

- **Token-generating separators**:  
  `(`, `)`, `{`, `}`, `[`, `]`, `;`, `,`
- **Ignored separators (do not generate tokens)**:  
  whitespace, newline `\n`, tab `\t`
