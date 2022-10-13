void Image::Read(const char *path) {
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);

    if (!f.is_open()) {
        std::cout << "File could not be opened 2\n";
        return;
    }

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char *>(fileHeader), fileHeaderSize);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        std::cout << "The specified path is not a bitmap image\n";
        f.close();
        return;
    }

    unsigned char informationHeader[fileHeaderSize];
    f.read(reinterpret_cast <char *> (informationHeader), informationHeaderSize);

    //int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) +
              (informationHeader[7] << 24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) +
               (informationHeader[11] << 24);

    m_colors.resize(m_width * m_height);

    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            unsigned char color[3];
            f.read(reinterpret_cast<char *>(color), 3);

            m_colors[y * m_width + x].r = static_cast<float>(color[2]) / 255.0f;
            m_colors[y * m_width + x].r = static_cast<float>(color[1]) / 255.0f;
            m_colors[y * m_width + x].r = static_cast<float>(color[0]) / 255.0f;
        }

        f.ignore(paddingAmount);
    }

    f.close();

    std::cout << "File read\n";
}


