set nu
set ts=4
set softtabstop=4
set shiftwidth=4
set expandtab
set iskeyword +=.

"set smartindent
"set autoindent

set cursorline
"highlight CursorLine cterm=NONE ctermbg=blue

inoremap {} {<CR>}<ESC>kA<CR>
inoremap () ()<ESC>ha

set foldmethod=syntax
set foldlevel=99
nnoremap <space> za

set tags=/workspace/git/ezgaoro/spitfire_atlas_r18b/tags
""""""""""""""""""""""""""""""
   " Tag list (ctags)
""""""""""""""""""""""""""""""
let Tlist_Ctags_Cmd = '/usr/bin/ctags'

let Tlist_Show_One_File = 1
let Tlist_Exit_OnlyWindow = 1
let Tlist_Use_Right_Window = 0
let Tlist_OnlyWindow=1

"let Tlist_Use_Right_Window=0
"let Tlist_Sort_Type='name'
"let Tlist_Show_Menu=1
"let Tlist_Max_Submenu_Items=10
"let Tlist_Max_Tag_length=16     "20
"let Tlist_Use_SingleClick=0
"let Tlist_Auto_Open=0
"let Tlist_Close_On_Select=0
"let Tlist_File_Fold_Auto_Close=1
"let Tlist_GainFocus_On_ToggleOpen=0
"let Tlist_Process_File_Always=1
"let Tlist_WinHeight=10
"let Tlist_WinWidth=18
"let Tlist_Use_Horiz_Window=0

let Tlist_Auto_Highlight_Tag = 1
"let Tlist_Auto_Open = 1
let Tlist_Auto_Update = 1
let Tlist_Close_On_Select = 0
let Tlist_Compact_Format = 0
let Tlist_Display_Prototype = 0
let Tlist_Display_Tag_Scope = 1
let Tlist_Enable_Fold_Column = 0
let Tlist_File_Fold_Auto_Close = 0
let Tlist_GainFocus_On_ToggleOpen = 1
let Tlist_Hightlight_Tag_On_BufEnter = 1
let Tlist_Inc_Winwidth = 0
let Tlist_Max_Submenu_Items = 1
let Tlist_Max_Tag_Length = 30
let Tlist_Process_File_Always = 0
let Tlist_Show_Menu = 0
let Tlist_Sort_Type = "order"
let Tlist_Use_Horiz_Window = 0
let Tlist_WinWidth = 31

map <F12> :TlistOpen<CR>
