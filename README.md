# PDFScreenplayParser
Parses a screenplay-formatted PDF document into an object of screenplay-typed elements

# Interface

This relies upon an **external** PDF parser / extractor, and expects a document struct that looks sort of like this:

```
Document: [
  Page1:[
    Line1: [
      Word1: {
        StringContent: "INT.",
        Position: (108.0, 100.10),
        FontName: "CourierBold",
        FontSize: 12.0,
      },
      Word2: {
        StringContent: "HOUSE",
        Position: (180.0, 100.10),
        FontName: "CourierBold",
        FontSize: 12.0,
      },
    ] 
  ]
  Page2: {},
  PageN...
]
```

By using the position, font name, and font size of each text fragment, we can determine what kind of Screenplay Element each fragment or line should be.

We return a new version of this struct, which is logically optimized, and assigns a specific type `SPType` to each individual `SPTextElement`. The returned ScreenplayDoc struct is ready for Screenplay-related processing and displaying.
