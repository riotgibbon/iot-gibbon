namespace deskmate {
    namespace arduino {
        namespace sensors {

            class sensor{
                public:
                virtual void read();
            };

             class dummy: public sensor{
                public:
                void read() override;
            };
        }
    }
}