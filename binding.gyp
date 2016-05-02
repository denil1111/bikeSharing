{
  "targets": [
    {
      "target_name": "bike",
      "include_dirs": [
        "/usr/local/include/",
        "<!(node -e \"require('nan')\")"
      ],
      "cflags": [
        "-O2"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "conditions": [
        [
          "OS=='mac'",
          {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
            },
            "libraries": [
              "-lemon",
              "-L /usr/local/lib/"
            ]
          }
        ],
        [
          "OS=='linux'",
          {
            "libraries": [
              "-llemon",
              "-L /usr/local/lib/"
            ]
          }
        ]
      ],
      "sources": [
        "src/test/commenhead.cpp",
        "src/test/capacitated.cpp",
        "src/test/uncapacitatedBSP.cpp",
        "src/test/supernode.cpp",
        "src/test/test.cpp",
        "src/test/ktimescapacitatedBSP.cpp",
        "src/test/tspbase.cpp",
        "src/test/noZeroCapacitatedBSP.cpp",
        "src/test/bspbase.cpp"
      ]
    }
  ]
}