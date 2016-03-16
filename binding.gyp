{
  "targets": [
    {
      "target_name": "bike",
      'include_dirs': [
          '/usr/local/include/',

        ],
      "libraries": [
      	"-lemon","-L /usr/local/lib/"
      ],
      "cflags": [ "-O2" ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ],
      "sources": [ 	"src/cpp/binding.cc",
      				"src/cpp/test.cpp"
      			 ]
    }
  ]
}

