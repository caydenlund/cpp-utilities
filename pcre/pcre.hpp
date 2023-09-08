/**
 * @file pcre.hpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @brief A header-only PCRE2-compatible regular expression engine.
 * @details
 *     ====================================================
 *     ||  Differences from this engine and true PCRE2:  ||
 *     ====================================================
 *     * The `\C` character class is not supported because it's inadvisable to use and not worth the effort to support.
 *     * Script run verification is not supported because it's an obscure feature and not worth the effort to support.
 *     * The pseudo-condition `VERSION` is not supported because this is not a version of the PCRE2 library.
 *     * Callouts (external function calls) are not supported because this library doesn't use the PCRE2 API.
 *
 *
 *
 *     =====================================
 *     ||  Regular expression reference:  ||
 *     =====================================
 *
 *     The PCRE2 pattern syntax is described in the
 *     [PCRE2 documentation](http://www.pcre.org/current/doc/html/pcre2pattern.html).
 *     A brief summary is provided below.
 *     Basic familiarity with regular expressions is assumed.
 *
 *
 *     Special characters:
 *     -------------------
 *     Inside of square brackets:
 *         * `\`  --  General escape character.
 *         * `^`  --  Negates the class, but only if the first character.
 *         * `-`  --  Indicates character range.
 *         * `[`  --  POSIX character class (if followed by POSIX syntax).
 *         * `]`  --  Terminates the character class.
 *     Outside of square brackets:
 *         * `\`  --  General escape character with several uses.
 *         * `^`  --  Assert start of string (or line, in multiline mode).
 *         * `$`  --  Assert end of string (or line, in multiline mode).
 *         * `.`  --  Match any character except newline (by default).
 *         * `[`  --  Start character class definition.
 *         * `|`  --  Start of alternative branch.
 *         * `(`  --  Start group or control verb.
 *         * `)`  --  End group or control verb.
 *         * `*`  --  0 or more quantifier.
 *         * `+`  --  1 or more quantifier; also "possessive quantifier".
 *         * `?`  --  0 or 1 quantifier; also quantifier minimizer.
 *         * `{`  --  Start min/max quantifier.
 *
 *
 *     Escaping special characters:
 *     ----------------------------
 *     * `\x`, where `x` is a special character.
 *     * `\Q...\E`, where `...` is any number of special characters.
 *
 *
 *     The backslash:
 *     --------------
 *     * Used to escape special characters.
 *       I.e., if followed by a character that is not a digit or letter, treats it as a character literal.
 *       (E.g., `\\` matches a literal backslash.)
 *     * Used to represent invisible characters.
 *         * `\a`           --  Alarm (the BEL character, 0x07).
 *         * `\cx`          --  Control-x, where `x` is any printable ASCII character.
 *         * `\e`           --  Escape (0x1B).
 *         * `\f`           --  Form feed (0x0C).
 *         * `\n`           --  New line (0x0A).
 *         * `\r`           --  Carriage return (0x0D).
 *         * `\t`           --  Horizontal tab (0x09).
 *         * `\0dd`         --  Character with octal code `dd`.
 *         * `\ddd`         --  Character with octal code `ddd`, or backreference.
 *         * `\o{ddd..}`    --  Character with octal code `ddd..`.
 *         * `\xhh`         --  Character with hexadecimal code `hh`.
 *         * `\x{hhh..}`    --  Character with hexadecimal code `hhh..`.
 *         * `\N{U+hhh..}`  --  Character with Unicode hex code point `hhh..`.
 *     * Used for back-references.
 *     * Used for specifying generic character types.
 *
 *
 *     Character types:
 *     ----------------
 *     * `.`   --  Any single character except newline.
 *     * `\d`  --  Any decimal digit.
 *     * `\D`  --  Any character that is not a decimal digit.
 *     * `\h`  --  Any horizontal white space character.
 *     * `\H`  --  Any character that is not a horizontal white space character.
 *     * `\N`  --  Any character that is not a newline.
 *     * `\p{xx}`  --  Any character with the Unicode property `xx`.
 *     * `\P{xx}`  --  Any character without the Unicode property `xx`.
 *     * `\R`  --  Any newline sequence. (`\n`, `\r`, `\f`, or `\r\n`).
 *     * `\s`  --  Any white space character.
 *     * `\S`  --  Any character that is not a white space character.
 *     * `\v`  --  Any vertical white space character.
 *     * `\V`  --  Any character that is not a vertical white space character.
 *     * `\w`  --  Any "word" character.
 *     * `\W`  --  Any "non-word" character.
 *     * `\X`  --  A Unicode extended grapheme cluster.
 *
 *
 *     Character properties for `\p` and `\P`:
 *     ---------------------------------------
 *     * `C`  --  Other.
 *     * `Cc` --  Control.
 *     * `Cf` --  Format.
 *     * `Cn` --  Unassigned.
 *     * `Co` --  Private use.
 *     * `Cs` --  Surrogate.
 *
 *     * `L`  --  Letter.
 *     * `Ll` --  Lower case letter.
 *     * `Lm` --  Modifier letter.
 *     * `Lo` --  Other letter.
 *     * `Lt` --  Title case letter.
 *     * `Lu` --  Upper case letter.
 *     * `L&` --  Ll, Lu, or Lt.
 *
 *     * `M`  --  Mark.
 *     * `Mc` --  Spacing mark.
 *     * `Me` --  Enclosing mark.
 *     * `Mn` --  Non-spacing mark.
 *
 *     * `N`  --  Number.
 *     * `Nd` --  Decimal number.
 *     * `Nl` --  Letter number.
 *     * `No` --  Other number.
 *
 *     * `P`  --  Punctuation.
 *     * `Pc` --  Connector punctuation.
 *     * `Pd` --  Dash punctuation.
 *     * `Pe` --  Close punctuation.
 *     * `Pf` --  Final punctuation.
 *     * `Pi` --  Initial punctuation.
 *     * `Po` --  Other punctuation.
 *     * `Ps` --  Open punctuation.
 *
 *     * `S`  --  Symbol.
 *     * `Sc` --  Currency symbol.
 *     * `Sk` --  Modifier symbol.
 *     * `Sm` --  Mathematical symbol.
 *     * `So` --  Other symbol.
 *
 *     * `Xan`  --  Alphanumeric: union of properties `L` and `N`.
 *     * `Xps`  --  POSIX space: property `Z` or tab, `NL`, `VT`, `FF`, or `CR`.
 *     * `Xsp`  --  POSIX space: property `Z` or tab, `NL`, `VT`, `FF`, or `CR`.
 *     * `Xuc`  --  Universally-named character: one that can be represented by a Universal Character Name.
 *     * `Xwd`  --  Perl word: property `Xan` or underscore.
 *
 *     * `Z`  --  Separator.
 *     * `Zl` --  Line separator.
 *     * `Zp` --  Paragraph separator.
 *     * `Zs` --  Space separator.
 *
 *
 *     Unicode extended grapheme clusters:
 *     -----------------------------------
 *     The `\X` matches any Unicode extended grapheme cluster.
 *     It treats the sequence as an atomic group.
 *
 *     First, it matches a single character.
 *     Then, it matches additional characters according to the following rules:
 *         * End at the end of the subject string.
 *         * Do not end between CR and LF; otherwise, end after any control character.
 *         * Do not break Hangul (a Korean script) syllable sequences.
 *           Hangul characters are of five types: `L`, `V`, `T`, `LV`, and `LVT`.
 *           An `L` character may be followed by an `L`, `V`, `LV`, or `LVT` character;
 *           an `LV` or `V` character may be followed by a `V` or `T` character;
 *           an `LVT` or `T` character may be followed only by a `T` character.
 *         * Do not end before extending characters or spacing marks or the "zero-width joiner" character.
 *           Characters with the "mark" property always have the "extend" grapheme breaking property.
 *         * Do not end after prepend characters.
 *         * Do not break within emoji modifier sequences or emoji zwj sequences.
 *           That is, do not break between characters with the Extended_Pictographic property.
 *           `Extend` and `ZWJ` characters are allowed between the characters.
 *         * Do not break within emoji flag sequences.
 *           That is, do not break between regional indicator (`RI`) characters if there are an odd number of `RI`
 *           characters before the break point.
 *         * Otherwise, end the cluster.
 *
 *
 *     POSIX named character classes:
 *     ------------------------------
 *     Syntax is `[[:class:]]` or `[[:^class]]`, where `class` is one of the following:
 *         * `alnum`  --  Alphanumeric.
 *         * `alpha`  --  Alphabetic.
 *         * `ascii`  --  `0`-`127`.
 *         * `blank`  --  Space or tab.
 *         * `cntrl`  --  Control character.
 *         * `digit`  --  Decimal digit.
 *         * `graph`  --  Printing, excluding space.
 *         * `lower`  --  Lower case letter.
 *         * `print`  --  Printing, including space.
 *         * `punct`  --  Printing, excluding alphanumeric.
 *         * `space`  --  White space.
 *         * `upper`  --  Upper case letter.
 *         * `word`   --  Same as `\w`.
 *         * `xdigit` --  Hexadecimal digit.
 *
 *
 *     Quantifiers:
 *     ------------
 *     * `?`       --  0 or 1, greedy.
 *     * `?+`      --  0 or 1, possessive.
 *     * `??`      --  0 or 1, lazy.
 *     * `*`       --  0 or more, greedy.
 *     * `*+`      --  0 or more, possessive.
 *     * `*?`      --  0 or more, lazy.
 *     * `+`       --  1 or more, greedy.
 *     * `++`      --  1 or more, possessive.
 *     * `+?`      --  1 or more, lazy.
 *     * `{n}`     --  Exactly `n`.
 *     * `{n,m}`   --  At least `n`, no more than `m`, greedy.
 *     * `{n,m}+`  --  At least `n`, no more than `m`, possessive.
 *     * `{n,m}?`  --  At least `n`, no more than `m`, lazy.
 *     * `{n,}`    --  `n` or more, greedy.
 *     * `{n,}+`   --  `n` or more, possessive.
 *     * `{n,}?`   --  `n` or more, lazy.
 *
 *
 *     Anchors and simple assertions:
 *     ------------------------------
 *     * `\b`  --  Word boundary.
 *     * `\B`  --  Not a word boundary.
 *     * `^`   --  Start of subject.
 *                 Also after an internal newline in multiline mode.
 *     * `\A`  --  Start of subject.
 *     * `$`   --  End of subject.
 *                 Also before newline at end of subject.
 *                 Also before internal newline in multiline mode.
 *     * `\Z`  --  End of subject.
 *                 Also before newline at end of subject.
 *     * `\z`  --  End of subject.
 *     * `\G`  --  First matching position in subject.
 *
 *
 *     Resetting the match start:
 *     --------------------------
 *     `\K` causes any previously-matched characters not to be included in the match.
 *     For example, the pattern `foo\Kbar` will match `bar` in `foobar`, but not `bar` in `barber`.
 *
 *     Captured substrings are not affected by this:
 *     `(foo)\Kbar` will match `bar` in `foobar`, and the first substring will be `foo`.
 *
 *     `\K` is forbidden in lookaround assertions.
 *
 *
 *     Notes on character classes:
 *     ---------------------------
 *     * `\b`, instead of matching a word boundary, matches the backspace character.
 *     * `\B`; `\R`; `\X`; and `\N` without an opening brace cause an error.
 *     * `-` defines a character range.
 *
 *
 *     Grouping:
 *     ---------
 *     * Capture groups are enclosed by parentheses `(`, `)`.
 *     * A group beginning with `(?:` does not create a capture group.
 *       (E.g., the first capture group of `(?:cata)(strophe)` is `(strophe)`.)
 *     * Option settings may be specified at the beginning of the group.
 *       E.g., `((?i)saturday)` matches `saturday`, `Saturday`, and `SATURDAY`.
 *     * Option settings in non-capturing groups may be specified in shorthand:
 *       `(?i:saturday)` matches `saturday`, `Saturday`, and `SATURDAY`.
 *     * If a group has alternatives, and the alternatives have sub-groups, these sub-groups may share the same number
 *       by beginning to super-group with a bar.
 *       In `(?|(abc)|(def))`, capture group `\1` refers to the matched `(abc)` or the matched `(def)`, depending on
 *       which group was matched.
 *     * Groups can be named in three ways:
 *           * `(?<name>...)`
 *           * `(?'name'...)`
 *           * `(?P<name>...)`
 *     * Group names must match the regular expression `[a-zA-Z_][a-zA-Z0-9_]{0,31}`.
 *     * If a group has alternatives, and the alternatives have named sub-groups, any of the names of these sub-groups
 *       refers to any sub-group of the same number.
 *       E.g., in the pattern `(?|(?<AA>abc)|(?<BB>def))`, both back-references `AA` and `BB` refer to the same
 *       captured group, either `abc` or `def` (depending on which group was matched).
 *     * Named capture groups do yield a capture group number, despite the question mark.
 *     * Names must be unique within a pattern, unless groups with the same name have the same number.
 *
 *
 *     Repetition:
 *     -----------
 *     Repetition is specified by a quantifier after any of the following items:
 *         * A literal data character.
 *         * `.`.
 *         * The `\C` escape sequence.
 *         * The `\R` escape sequence.
 *         * The `\X` escape sequence.
 *         * An escape such as `\d` or `\pL` that matches a single character.
 *         * A character class.
 *         * A backreference.
 *         * A parenthesized group (including lookaround assertions).
 *         * A subroutine call (recursive or otherwise).
 *
 *     A quantifier can be any of the following:
 *         * `?`       --  0 or 1.
 *         * `*`       --  0 or more.
 *         * `+`       --  1 or more.
 *         * `{n}`     --  Exactly `n`.
 *         * `{n, m}`  --  At least `n`, no more than `m`.
 *         * `{n, }`   --  At least `n`.
 *         * `{, m}`   --  At most `m`.
 *
 *     Quantifiers are greedy by default.
 *     Any quantifier may be followed by `?` to make it non-greedy (minimizing).
 *
 *     When a quantifier is used on a capture group, the captured group is the last iteration in the repetition.
 *
 *
 *     Atomic grouping:
 *     ----------------
 *     Consider the pattern `\d+foo` and the subject string `123456bar`.
 *     After matching all 6 digits and then failing to match "foo",
 *     the normal action of the matcher is to try again with only 5 digits matching the \d+ item, and then with 4,
 *     and so on, before ultimately failing.
 *
 *     "Atomic grouping" provides the means for specifying that once a group has matched,
 *     it is not to be re-evaluated in this way.
 *     If we use atomic grouping for the previous example,
 *     the matcher gives up immediately on failing to match "foo" the first time.
 *
 *     Atomic groups begin with `(?>`, or with `(*atomic:`. They don't create capture groups.
 *     An atomic group for the example above can be written as `(?>\d+)foo`, or `(*atomic:\d+)foo`.
 *
 *     Possessive quantifiers (suffix '+') are semantically equivalent to atomic grouping for a single repetition.
 *     The example above can be written as `\d++foo`.
 *
 *
 *     Back-references:
 *     ----------------
 *     A back-reference in a pattern outside of a character class can be written in the following ways:
 *         * `\n`, 1 <= n <= 7, where `n` is the number of the back-reference.
 *         * `\gn` or `\g{n}`, 1 <= n, where `n` is the number of the back-reference.
 *         * `\g{-n}`, 1 <= n, where `n` is the number of back-references to the left.
 *           I.e., it refers to the nth earlier capture group.
 *         * `\g{+n}`, 1 <= n, where `n` is the number of back-references to the right.
 *           I.e., it refers to the nth later capture group.
 *         * `\g{name}`, where `name` is the name of the referenced capture group.
 *         * `\k{name}`, where `name` is the name of the referenced capture group.
 *         * `\k<name>`, where `name` is the name of the referenced capture group.
 *         * `\k'name'`, where `name` is the name of the referenced capture group.
 *         * `(?P=name)`, where `name` is the name of the referenced capture group.
 *
 *     Recursive back-references (i.e., back-references that reference themselves) always fail on the first iteration.
 *     However, they can be used with alternation or a quantifier with a minimum of zero.
 *     For example, `(A\1)` never matches, but `(A|B\1)+` matches `A`, `ABA`, `ABBA`, `ABBAA`, etc.
 *
 *
 *     Assertions:
 *     -----------
 *     An assertion doesn't consume characters, but cause matches to fail when a condition is not met.
 *     There are eight simple assertions:
 *         * `\b`  --  Word boundary.
 *         * `\B`  --  Not a word boundary.
 *         * `^`   --  Start of subject.
 *                     Also after an internal newline in multiline mode.
 *         * `\A`  --  Start of subject.
 *         * `$`   --  End of subject.
 *                     Also before newline at end of subject.
 *                     Also before internal newline in multiline mode.
 *         * `\Z`  --  End of subject.
 *                     Also before newline at end of subject.
 *         * `\z`  --  End of subject.
 *         * `\G`  --  First matching position in subject.
 *
 *     Other assertions include positive and negative lookahead and lookbehind assertions.
 *     These are like non-capturing groups, but don't consume any characters.
 *     These are atomic by default.
 *         * `(*positive_lookbehind:...)`, `(*plb:...)`, or `(?<=...)`: positive lookbehind assertion.
 *           The pattern `(?<=foo)bar` matches `bar` if it is preceded by `foo`.
 *         * `(*negative_lookbehind:...)`, `(*nlb:...)`, or `(?<!...)`: negative lookbehind assertion.
 *           The pattern `(?<!foo)bar` matches `bar` if it is not preceded by `foo`.
 *         * `(*positive_lookahead:...)`, `(*pla:...)`, or `(?=...)`: positive lookahead assertion.
 *           The pattern `foo(?=bar)` matches `foo` if it is followed by `bar`.
 *         * `(*negative_lookahead:...)`, `(*nla:...)`, or `(?!...)`: negative lookahead assertion.
 *           The pattern `foo(?!bar)` matches `foo` if it is not followed by `bar`.
 *
 *     Lookbehinds have an extra restriction: all strings it matches must have a fixed length.
 *     An exception is that top-level branches may have different lengths from each other.
 *         * `(?<=abc|1234)` is permitted. It has top-level branches of different lengths from each other.
 *         * `(?<=abc(def|1234))` is not permitted. The branches are not of the top level.
 *         * `(?<=abc?)` is not permitted. Quantifiers are not allowed.
 *     Note that using `\K` to reset the match start does not have this restriction.
 *     Lookbehinds can call groups as subroutines so long as the subroutines have a fixed length.
 *     Lookbehinds can use back-references, as long as the following conditions are met:
 *         * The back-reference is not recursive.
 *         * There is no use of `(?|` in the pattern.
 *         * If the back-reference is by name, the name must be unique.
 *         * The referenced group must match a fixed-length substring.
 *
 *     Assertions may be chained.
 *     `(?<=\d{3})(?<!999)foo` matches `foo` if it is preceded by a three-digit number which is not `999`.
 *
 *     Assertions may be nested.
 *     `(?<=(?<!foo)123)bar` matches `bar` if it is preceded by `123` which is not preceded by `foo`.
 *     E.g., `bar` does not match; `123bar` matches; `bar123bar` matches; `foo123bar` does not match.
 *
 *     Non-atomic positive lookaround assertions exist:
 *         * `(*non_atomic_positive_lookbehind:...)`, `(*naplb:...)`, or `(?<*...)`:
 *           non-atomic positive lookbehind assertion.
 *         * `(*non_atomic_positive_lookahead:...)`, `(*napla:...)`, or `(?*...)`:
 *           non-atomic positive lookahead assertion.
 *     Consider the following example:
 *     We have a string of words, `foo bar baz baz bar 123`.
 *     We want to match the rightmost repeated word in the string.
 *     (I.e., of all the words in the string that appear more than once, match the rightmost one.)
 *     In this case, it should match the second instance of `bar`.
 *     The pattern `^(?<*.*\b(\w++))(?>.*?\b\1\b){2}` matches such a word.
 *         * `^` anchors the beginning of the string.
 *         * `(?<*` begins the non-atomic positive lookbehind assertion.
 *             * `.*` matches any number of preceding characters.
 *             * `\b(\w++)` matches a full word (`++` for atomicity).
 *         * `)` to end the lookbehind assertion.
 *         * `(?>` starts an atomic group.
 *             * `.*?` matches any number of characters in a non-greedy manner.
 *             * `\b\1\b` matches the word from the lookbehind assertion, surrounded by word boundaries.
 *         * `)` to end the atomic group.
 *         * `{2}` means that the word must appear at least twice.
 *
 *
 *     Conditional groups:
 *     -------------------
 *     The pattern can choose between two alternative paths depending on the result of an assertion,
 *     or whether a specific capture group has already been matched.
 *     `(?(condition)yes)` matches `yes` if `condition` is true. (The `no` pattern is the empty string.)
 *     `(?(condition)yes|no)` matches `yes` if `condition` is true, and `no` otherwise.
 *
 *     There are four kinds of conditions:
 *         * References to capture groups.
 *           `(?(group)...)`, where `group` is a group name (`<name>`, `'name'`, or (deprecated) `name`),
 *           absolute number (`n`), or relative number (`-n`, `+n`).
 *           Returns true if the given capture group has been matched.
 *         * References to recursion.
 *           `(?(Rgroup)...)`, where `group` is a group name (`&name`), number (`n`), or the empty string (``).
 *           Returns true if the following conditions are met:
 *               * `Rgroup` is not the name of any existing capture group.
 *                 (Otherwise, it behaves as a reference to that group.)
 *               * The current evaluation is inside a recursive call.
 *               * If `group` is not the empty string, the current recursive evaluation is inside the given group.
 *           For example, `((?(R1)abc|\1123))` on `abc123` matches the entire string; during the first iteration, the
 *           pattern matches `123`, and then evaluates `\1` recursively, where it matches the preceding `abc`.
 *         * The pseudo-condition `DEFINE`.
 *           This always returns false, even if there is a group with the name `DEFINE`.
 *           In addition, there may not be a `no` condition (i.e., control always skips this part of the pattern).
 *           This is used to define a new subroutine.
 *           For example, `(?(DEFINE)(?<byte>25[0-5]|2[0-4]\d|[0-1]?\d?\d))` defines a subroutine named `byte`,
 *           which matches numbers `0`-`255`, with optional leading zeroes.
 *           The rest of the pattern can then refer to this subroutine:
 *           `\b(?&byte)(\.(?&byte)){3}\b` matches an IP address made of four bytes delimited by periods.
 *         * An atomic positive or negative lookaround assertion.
 *
 *
 *     Comments:
 *     ---------
 *     Comments take the form `(?#comment)`. They may not contain nested parentheses.
 *
 *
 *     Recursion:
 *     ----------
 *     Subroutines may call themselves recursively.
 *     `(?n)` calls the subroutine `n`, where `n` is one of the following:
 *         * A group number (`n` or `-n`).
 *         * A group name (`&name`).
 *         * `R` or `0`. This calls the entire pattern recursively.
 *     For example, `\((\d++|(?R))\)` matches `123`, `(123)`, `((123))`, . . . .
 *
 *     Note that if a capture group is not set in a top-level group, its final capture value is not saved, even if it is
 *     captured in a recursive call.
 *    For example, `^((.)(?1)\2|.?)$` matches palindromic strings.
 *        * `^` matches the beginning of the string.
 *        * `(` begins the first capture group.
 *            * `(.)` matches any single character. This is the second capture group.
 *            * `(?1)` calls the first capture group recursively.
 *            * `\2` matches the second capture group. This is the single character from the beginning.
 *            * `|.?`: if the first option doesn't match here, then this is either a single character or nothing.
 *              (This is the midpoint of the string.)
 *        * `)` ends the first capture group.
 *        * `$` matches the end of the string.
 *
 *
 *    Subroutines:
 *    ------------
 *    A subroutine is similar to a back-reference, except that instead of copying the specific match,
 *    it copies the matching pattern.
 *    The syntax is as follows:
 *        * `(?group)`, where `group` is one of the following:
 *            * A group number (`n`, `-n`, or `+n`).
 *            * A group name (`&name` or `P>name`).
 *        * Oniguruma's `\g<group>` or `\g'group'`, where `group` is one of the following:
 *            * A group number (`n`, `-n`, or `+n`).
 *            * A group name (`name`).
 *
 *    Note that options don't apply to subroutines;
 *    in `(abc)(?i:(?-1))`, the pattern would match `abc`, but not `Abc`, even though the `i` option is present.
 *
 *
 *    Backtracking control verbs:
 *    ---------------------------
 *    TODO: Document this.
 *
 *
 *    Options:
 *    --------
 *    TODO: Document this.
 *          Include both beginning-of-string and in-group options.
 *
 *
 *    Substitution:
 *    -------------
 *    TODO: Document this.
 *
 *
 * @license MIT license (https://opensource.org/licenses/MIT).
 * @copyright (c) 2023 by Cayden Lund.
 *
 */

#ifndef PCRE_HPP
#define PCRE_HPP

#include <string>
#include <vector>

namespace util {

    /**
     * @brief A PCRE2-compatible regular expression engine.
     * @details
     *     Usage is as follows:
     *     --------------------
     *
     *     First, compile a pattern from a string.
     *
     *     ```
     *     util::pcre pattern("(foo|bar)" + "\(" + "(?<args>.*)" + "\)");
     *     ```
     *
     *     Match a string against the compiled pattern using the
     *     `::operator(subject, index)`, `::match(subject, index)`, or `::match_all(subject, index)` function.
     *     There are five forms:
     *         * ```
     *           util::pcre::match_result util::pcre::operator()(const std::string &subject,
     *                                                           size_t start_index = 0) const;
     *           ```
     *           Member function call operator on a pattern. Returns the first match from the `start_index`.
     *
     *         * ```
     *           util::pcre::match_result util::pcre::match(const std::string &subject,
     *                                                      size_t start_index = 0) const;
     *           ```
     *           Member function on a pattern. Returns the first match from the `start_index`.
     *
     *         * ```
     *           static util::pcre::match_result util::pcre::match(const util::pcre& pattern,
     *                                                             const std::string &subject,
     *                                                             size_t start_index = 0) const;
     *           ```
     *           Static function. Returns the first match from the `start_index`.
     *
     *         * ```
     *           std::vector<util::pcre::match_result> util::pcre::match_all(const std::string &subject) const;
     *           ```
     *           Member function on a pattern. Returns all matches from the `start_index`.
     *
     *         * ```
     *           static std::vector<util::pcre::match_result> util::pcre::match_all(const util::pcre& pattern,
     *                                                                              const std::string &subject) const;
     *           ```
     *           Static function. Returns all matches from the `start_index`.
     *
     *     ```
     *     util::pcre::match_result pat_match = pattern.match("foo(x, y, z)");
     *     //  Could also use `pat_match = util::pcre::match(pattern, "foo(x, y, z)")`
     *     //  or `pat_match = pattern("foo(x, y, z)")`.
     *     ```
     *
     *     Retrieve capture groups from the match using the subscript operator.
     *
     *     ```
     *     const std::string full_match = pat_match[0];  //  "foo(x, y, z)". Could also use `full_match = pat_match()`.
     *     const std::string name = pat_match[1];        //  "foo".
     *     const std::string args = pat_match["args"];   //  "x, y, z". Could also use `pat_match[2]`.
     *     ```
     *
     *     Perform a regex substitution against the compiled pattern using the `util::pcre::sub()` function.
     *     There are two forms:
     *         * ```
     *           std::string util::pcre::sub(const std::string &subject,
     *                                       const std::string& replacement,
     *                                       size_t start_index = 0) const;
     *           ```
     *           Member function on a pattern. Replaces the first match from the `start_index`.
     *
     *         * ```
     *           static std::string util::pcre::sub(const util::pcre& pattern,
     *                                              const std::string& subject,
     *                                              const std::string& replacement,
     *                                              size_t start_index = 0) const;
     *           ```
     *           Static function. Replaces the first match from the `start_index`.
     *
     *         * ```
     *           std::string util::pcre::sub_all(const std::string &subject,
     *                                           const std::string& replacement,
     *                                           size_t start_index = 0) const;
     *           ```
     *           Member function on a pattern. Replaces all matches from the `start_index`.
     *
     *         * ```
     *           static std::string util::pcre::sub_all(const util::pcre& pattern,
     *                                                  const std::string& subject,
     *                                                  const std::string& replacement,
     *                                                  size_t start_index = 0) const;
     *           ```
     *           Static function. Replaces all matches from the `start_index`.
     *
     *
     *     ```
     *     std::cout << pattern.sub("ABC foo(x, y, z) DEF", "XYZ");                        //  "ABC XYZ DEF".
     *     std::cout << util::pcre::sub(pattern, "foo(x, y, z)", "\\2");                   //  "x, y, z".
     *     std::cout << pattern.sub_all("foo(a, b, c) bar(d, e, f) baz(g, h, i)", "---");  //  "--- --- baz(g, h, i)"
     *     ```
     */
    class pcre {
    public:
        /**
         * @brief Compiles a new `pcre` regular expression object.
         *
         * @param pattern The regular expression to match.
         * @param options A string of options. Optional.
         */
        pcre(const std::string& pattern, const std::string& options = "");

        /**
         * @brief The result of a `::match(subject, index)` call.
         * @details
         *     Contains the matched string as well as all the subgroups of the pattern.
         *     If the pattern matched nothing, then the following are true:
         *         * `::size() == 0`.
         *         * `::begin() == ::end()`.
         *         * `::found_match() == false`.
         *         * `::empty() == true`.
         *         * `::operator()() == ""`.
         *         * `::operator[](...) == ""`.
         *
         *     The entire captured string is accessible via either the function call operator `::operator()()`, or the
         *     subscript operator by group index `::operator[](group_index)` or group name `::operator[](group_name)`.
         *
         *     The `::size()` method returns the number of subgroups, plus one (for the entire string).
         *
         *     The `::begin()` and `::end()` methods return an iterator pointing to the beginning and end of a vector
         *     of the entire string and then all substrings.
         *
         *     `::empty()` reports whether the pattern matched nothing (i.e., `false` if matched);
         *     `::found_match()` reports whether the pattern matched something (i.e., `true` if matched).
         */
        class match_result {
        public:
            /**
             * @brief Returns the entire matched string.
             *
             * @return The entire matched string; the empty string if the pattern matched nothing.
             */
            std::string operator()() const;

            /**
             * @brief Returns the matched subgroup string for the given group index.
             * @details Returns the entire matched string if the given group index is 0.
             *
             * @param group_index The index of the subgroup to return.
             * @return The matched string of the given group index.
             */
            std::string operator[](size_t group_index) const;

            /**
             * @brief Returns the matched subgroup string for the given group name.
             *
             * @param group_name The name of the subgroup to return.
             * @return The matched string of the given group name.
             */
            std::string operator[](const std::string& group_name) const;
        };

        /**
         * @brief Function call operator to match a string against the compiled pattern.
         *
         * @param subject The subject string to match.
         * @param start_index The start index from which to match. Optional.
         * @return The result of the match operation.
         */
        match_result operator()(const std::string& subject, size_t start_index = 0) const;

        /**
         * @brief Matches the given string against the compiled pattern.
         *
         * @param subject The subject string to match.
         * @param start_index The start index from which to match. Optional.
         * @return The result of the match operation.
         */
        match_result match(const std::string& subject, size_t start_index = 0) const;

        /**
         * @brief Matches the given string against the given compiled pattern.
         *
         * @param pattern The compiled pattern to use.
         * @param subject The subject string to match.
         * @param start_index The start index from which to match. Optional.
         * @return The result of the match operation.
         */
        static match_result match(const pcre& pattern, const std::string& subject, size_t start_index = 0);

        /**
         * @brief Matches the given string against the compiled pattern.
         *
         * @param subject The subject string to match.
         * @param start_index The start index from which to match. Optional.
         * @return All results of the match operation.
         */
        std::vector<match_result> match_all(const std::string& subject, size_t start_index = 0) const;

        /**
         * @brief Matches the given string against the given compiled pattern.
         *
         * @param pattern The compiled pattern to use.
         * @param subject The subject string to match.
         * @param start_index The start index from which to match. Optional.
         * @return All results of the match operation.
         */
        static std::vector<match_result> match_all(const pcre& pattern, const std::string& subject,
                                                   size_t start_index = 0);

        /**
         * @brief Replaces the first match of the compiled pattern with the given replacement.
         *
         * @param subject The subject string to match and modify.
         * @param replacement The replacement pattern.
         * @param start_index The start index from which to match. Optional.
         * @return The result of the substitution operation.
         */
        std::string replace(const std::string& subject, const std::string& replacement, size_t start_index = 0) const;

        /**
         * @brief Replaces the first match of the given compiled pattern with the given replacement.
         *
         * @param pattern The compiled pattern to use.
         * @param subject The subject string to match and modify.
         * @param replacement The replacement pattern.
         * @param start_index The start index from which to match. Optional.
         * @return The result of the substitution operation.
         */
        static std::string replace(const pcre& pattern, const std::string& subject, const std::string& replacement,
                                   size_t start_index = 0);

        /**
         * @brief Replaces all matches of the compiled pattern with the given replacement.
         *
         * @param subject The subject string to match and modify.
         * @param replacement The replacement pattern.
         * @param start_index The start index from which to match. Optional.
         * @return The result of the substitution operation.
         */
        std::string replace_all(const std::string& subject, const std::string& replacement,
                                size_t start_index = 0) const;

        /**
         * @brief Replaces all matches of the given compiled pattern with the given replacement.
         *
         * @param pattern The compiled pattern to use.
         * @param subject The subject string to match and modify.
         * @param replacement The replacement pattern.
         * @param start_index The start index from which to match. Optional.
         * @return The result of the substitution operation.
         */
        static std::string replace_all(const pcre& pattern, const std::string& subject, const std::string& replacement,
                                       size_t start_index = 0);
    };

}  //  namespace util

#endif  //  PCRE_HPP
