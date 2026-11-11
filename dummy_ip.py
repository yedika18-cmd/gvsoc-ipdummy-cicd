import os
import gvsoc.systree as st

# Membuat class dummyip sebagai componen GVSoC #
class DummyIp(st.Component):

    def __init__(self, parent, name):
        super().__init__(parent, name)
        
        # Mendaftarkan file dummy_ip.cpp sebagai code C++ komponen DummyIP #
        self.add_sources([os.path.join(os.path.dirname(__file__), "dummy_ip.cpp")])
        
#        self.add_sources([
#            os.path.join(
#                os.path.dirname(__file__),
#                'dummy_ip.cpp'
#            )
#        ])

# membuat fungsi untuk menyediakan interface input dummyip
    def i_INPUT(self):
        return st.SlaveItf(
            self,   # ini mengacu kepada file ini
            'input',  # nama port yang di daftarkan pada dummy_ip.cpp
            signature='oke'  
        )


