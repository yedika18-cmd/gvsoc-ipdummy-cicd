
#include <vp/vp.hpp>
#include <vp/itf/io.hpp>

#include <iostream>
#include <cstdint>

// =======================================
// DEKLARASI CLASS TESTMASTER
// =======================================

class TestMaster : public vp::Component
{
public:
    TestMaster(vp::ComponentConf &config);

    void reset(bool active);

private:
    vp::IoMaster out_itf;
};

// =======================================
// CONSTRUCTOR TESTMASTER
// =======================================

TestMaster::TestMaster(vp::ComponentConf &config)
    : vp::Component(config)
{
    // Membuat master port bernama "out"
    this->new_master_port(
        "out",
        &this->out_itf
    );
}

// =======================================
// FUNGSI RESET DAN PENGUJIAN
// =======================================

void TestMaster::reset(bool active)
{
    // Saat reset aktif, pengujian tidak dijalankan
    if (active)
    {
        return;
    }

    std::cout
        << "[TestMaster] Starting Echo Test..."
        << std::endl;

    // ===================================
    // MENENTUKAN NILAI TX DAN RX
    // ===================================

    uint32_t tx_data = 0x182005;
    uint32_t rx_data = 0;

    // ===================================
    // MEMBUAT TRANSAKSI WRITE
    // ===================================

    vp::IoReq write_req;

    write_req.init();

    write_req.set_addr(0x0);

    write_req.set_size(sizeof(tx_data));

    write_req.set_data(
        reinterpret_cast<uint8_t *>(&tx_data)
    );

    // Menentukan operasi WRITE
    write_req.set_is_write(true);

    // Mengirim data ke Dummy IP
    vp::IoReqStatus write_status =
        this->out_itf.req(&write_req);

    // Memeriksa hasil transaksi WRITE
    if (write_status != vp::IO_REQ_OK)
    {
        std::cout
            << "[ERROR] WRITE failed"
            << std::endl;

        // Mengakhiri simulasi dengan status gagal
        this->time.get_engine()->quit(1);

        return;
    }

    // ===================================
    // MEMBUAT TRANSAKSI READ
    // ===================================

    vp::IoReq read_req;

    read_req.init();

    read_req.set_addr(0x0);

    read_req.set_size(sizeof(rx_data));

    read_req.set_data(
        reinterpret_cast<uint8_t *>(&rx_data)
    );

    // Menentukan operasi READ
    read_req.set_is_write(false);

    // Membaca data dari Dummy IP
    vp::IoReqStatus read_status =
        this->out_itf.req(&read_req);

    // Memeriksa hasil transaksi READ
    if (read_status != vp::IO_REQ_OK)
    {
        std::cout
            << "[ERROR] READ failed"
            << std::endl;

        // Mengakhiri simulasi dengan status gagal
        this->time.get_engine()->quit(1);

        return;
    }

    // ===================================
    // MENAMPILKAN NILAI TX DAN RX
    // ===================================

    std::cout
        << "[TestMaster] TX = 0x"
        << std::hex
        << tx_data
        << std::endl;

    std::cout
        << "[TestMaster] RX = 0x"
        << rx_data
        << std::dec
        << std::endl;

    // ===================================
    // MEMBANDINGKAN DATA TX DAN RX
    // ===================================

    if (tx_data == rx_data)
    {
        std::cout
            << "================================="
            << std::endl;

        std::cout
            << "[PASS] Dummy IP Echo/Loopback OK!"
            << std::endl;

        std::cout
            << "================================="
            << std::endl;

        // Mengakhiri simulasi dengan status berhasil
        this->time.get_engine()->quit(0);
    }
    else
    {
        std::cout
            << "[FAIL] Dummy IP Echo/Loopback"
            << std::endl;

        // Mengakhiri simulasi dengan status gagal
        this->time.get_engine()->quit(1);
    }
}

// =======================================
// REGISTRASI KOMPONEN KE GVSOC
// =======================================

extern "C"
vp::Component *gv_new(vp::ComponentConf &config)
{
    return new TestMaster(config);
}

