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
            "cflags_cc": ["-fexceptions"]
        }
    ]
}
