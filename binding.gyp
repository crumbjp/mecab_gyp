{
    "targets": [
        {
            "target_name": "mecab_gyp",
            "sources": ["src/mecab_gyp.cc"],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "<!(mecab-config --inc-dir)"
            ],
            "libraries": ["<!(mecab-config --libs)"],
            "cflags_cc": ["-fexceptions"],
            'xcode_settings': {
                'MACOSX_DEPLOYMENT_TARGET': '10.7',
                'OTHER_CFLAGS': [
                    "-std=c++11",
                    "-stdlib=libc++"
                ],
            },
        }
    ]
}
