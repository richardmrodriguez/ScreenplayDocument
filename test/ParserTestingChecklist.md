# PDF Parser

## Line Types

- ACTION
- DIALOGUE

## TODO - Line Types

- SCENE NUMBER
    - Optional (TODO - handle AUTOMATIC SCENE NUMBERING)
    - On left and right-hand sides of SCENE HEADINGS
    - Alphanumeric string (1, 3B, 25C, etc.)

- PAGE NUMBER
    - Alphanumeric string (1., 3B., etc.)
    - Ends in `.`
    - First page of screenplay content may omit number for page 1 (TODO)

- PARENTHETICAL
    - Could have whitespace

- CHARACTER
    - CHARACTER NAME
        - Name could be long / have WHITESPACE, etc. (JOHN I) (TODO)
    - CHARACTER EXTENSION(S) [(O.S.), (V.O.), etc.]
        - Could have WHITESPACE (FROM VENT), etc.

- SCENE HEADINGS (TODO)
    - LOCATION
    - SUBLOCATION(S) (TODO)
    - TIME OF DAY (TODO)
        - DAY, NIGHT, MORNING, etc.
    - SCENE HEADING SEPARATOR (hyphen) (TODO)
    - TIMEFRAME (PAST, FUTURE, arbitrary [1950s], etc.)

- REVISION MARKER (TODO)
    - Usually right-hand side only (according to FINAL DRAFT...)
    - default position is 7.75" from left-hand side (558 pts @ 72ppi)
    - default marker is single asterisk in Final Draft `*`

- PAGE REVISION HEADER
    - Get revision COLOR
    - Get revision DATE from string into a unix timestamp

- MORE / CONTINUED
    - PAGE more/continued
        - NOT dialogue
    - DIALOGUE more/continued
        - Part of CHARACTER / EXTENSION??
        - DOESN'T COUNT as dialogue per se

- TITLE PAGE ELEMENTS
    - TITLE
    - AUTHOR(S)
    - BYLINE
    - BASED ON / ETC.
    - REVISION(S)
    - CONTACT INFO
    - COPYRIGHT / WGA / OTHER
