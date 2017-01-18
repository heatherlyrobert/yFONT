printf "txf_make - create all texture-mapped font files for yFONT at once\n"

if [ "$(whoami)" != "root"  ]; then
   printf "FAIL : must run as root\n"
   exit
fi


BASE="/home/shared/fonts"
INST="/usr/local/share/fonts"

function  install_font {
   printf "   - $1\n"
   txf_make --norm --point 40 --adjust ${2} --file "${INST}/${1}.txf" --font "${3}" > /dev/null
   chmod 0644 "${INST}/$1.txf"
}

function  install_mand {
   printf "   - $1\n"
   txf_make --mandfreq --point 24 --adjust ${2} --file "${INST}/${1}.txf" --font "${3}" > /dev/null
   chmod 0644 "${INST}/$1.txf"
}

function  install_lower {
   printf "   - $1\n"
   txf_make --lower --point 24 --adjust ${2} --file "${INST}/${1}.txf" --font "${3}" > /dev/null
   chmod 0644 "${INST}/$1.txf"
}

function  install_tsae {
   printf "   - $1\n"
   txf_make --tsae --point 24 --adjust ${2} --file "${INST}/${1}.txf" --font "${3}" > /dev/null
   chmod 0644 "${INST}/$1.txf"
}

function  install_small {
   printf "   - $1\n"
   txf_make --norm --point  7 --adjust ${2} --file "${INST}/${1}.txf" --font "${3}" > /dev/null
   chmod 0644 "${INST}/$1.txf"
}

function  install_tiny  {
   printf "   - $1\n"
   txf_make --norm --point  6 --adjust ${2} --file "${INST}/${1}.txf" --font "${3}" > /dev/null
   chmod 0644 "${INST}/$1.txf"
}


printf "\nprepare...\n"
printf "   - remove existing fonts\n"
rm -f "${INST}/*.txf"


# emerge media-fonts/corefonts
printf "\ncorefonts...\n"
install_font  "arial"          0  "/usr/share/fonts/corefonts/arial.ttf"
install_small "arial_sm"       0  "/usr/share/fonts/corefonts/arial.ttf"
install_tiny  "arial_tiny"     0  "/usr/share/fonts/corefonts/arial.ttf"
install_font  "comic"          0  "/usr/share/fonts/corefonts/comic.ttf"
install_font  "courier"        0  "/usr/share/fonts/corefonts/cour.ttf"
install_small "courier_sm"     0  "/usr/share/fonts/corefonts/cour.ttf"
install_font  "verdana"       -4  "/usr/share/fonts/corefonts/verdana.ttf"
install_font  "verdana_bold"  -4  "/usr/share/fonts/corefonts/verdanab.ttf"
install_small "verdana_sm"     0  "/usr/share/fonts/corefonts/verdana.ttf"
install_tiny  "verdana_tiny"   0  "/usr/share/fonts/corefonts/verdana.ttf"

# my personal supply of fonts
printf "\npersonal fonts...\n"
install_font  "aduren"        -2  "${BASE}/aduren.ttf"
install_font  "clarity"        0  "${BASE}/clarity.ttf"
install_font  "coolvetica"     1  "${BASE}/coolvetica.ttf"
install_font  "comfortaa"      1  "${BASE}/comfortaa_regular.ttf"
install_font  "fifa"           8  "${BASE}/fifa_welcome.ttf"
install_font  "harb"           0  "${BASE}/harb.ttf"
install_font  "jackinput"      1  "${BASE}/jackinput.ttf"
install_font  "lucida_casual"  0  "${BASE}/lucida_casual.ttf"
install_font  "moderna"        0  "${BASE}/moderna.ttf"
install_font  "monofur"        4  "${BASE}/monofur.ttf"
install_font  "paola"         -2  "${BASE}/paola.ttf"
install_font  "papyrus"        6  "${BASE}/papyrus.ttf"
install_font  "peabrain"     -16  "${BASE}/peabrain.ttf"
install_font  "providence"     0  "${BASE}/providence.ttf"
install_font  "scriptina"      8  "${BASE}/SCRIPTIN.ttf"
install_font  "sketch"         0  "${BASE}/sketch_block.ttf"
install_font  "space_age"      0  "${BASE}/space_age.ttf"
install_font  "subpear"        1  "${BASE}/subpear.ttf"
install_font  "teacher"       -4  "${BASE}/teacher.ttf"
install_font  "teacher_k"     -2  "${BASE}/teacher-k.ttf"
install_font  "yes"           -6  "${BASE}/yes_songofseven.ttf"
install_font  "zygo"          -2  "${BASE}/ZYGO.ttf"

# my personal foreign supply of fonts
printf "\nforeign fonts...\n"
install_mand  "simhei"         0  "${BASE}/simhei.ttf"
#install_font  "hiragana"       0  "${BASE}/hiragana.ttf"
#install_font  "moonbeams"      0  "${BASE}/moonbeams.ttf"
#install_font  "coobblk"        0  "${BASE}/coobblk.ttf"
#install_font  "coobblok"       0  "${BASE}/coobblok.ttf"
#install_font  "autobot"        0  "${BASE}/autobot.ttf"
#install_font  "d3cozmism"      0  "${BASE}/d3cozmism.ttf"

# hand designed by us
printf "\nhandcrafted tsae fonts...\n"
install_tsae  "tsa_braille"    0  "${BASE}/tsa_braille.v15.ttf"
install_tsae  "tsa_mayan"      0  "${BASE}/tsa_mayan.v91.ttf"
install_tsae  "tsa_klinzhai"   0  "${BASE}/tsa_klinzhai.v77.ttf"
install_tsae  "tsa_borg"       0  "${BASE}/tsa_borg.v20.ttf"
install_tsae  "tsa_ring"       0  "${BASE}/tsa_ring.v04.ttf"
install_tsae  "tsa_script"     0  "${BASE}/tsa_script.v16.ttf"
install_tsae  "tsa_cree"       0  "${BASE}/tsa_cree.v19.ttf"

printf "\ncomplete\n"


