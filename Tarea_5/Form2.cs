using System;
using System.Drawing;
using System.Windows.Forms;

namespace Tarea_5
{
    public partial class Form2 : Form
    {
        // 2–pasillo–2  => A B  [ ]  C D
        private readonly string[] seatCols = { "A", "B", "", "C", "D" }; // "" = pasillo
        private const int totalRows = 7;

        private Button asientoSeleccionado = null;

        public Form2()
        {
            InitializeComponent();
            this.Load += Form2_Load;
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            // Bordes visibles entre celdas
            tlpSeats.CellBorderStyle = TableLayoutPanelCellBorderStyle.Single;

            // Definir columnas (5: A, B, PASILLO, C, D)
            tlpSeats.SuspendLayout();
            tlpSeats.Controls.Clear();

            tlpSeats.ColumnStyles.Clear();
            tlpSeats.ColumnCount = seatCols.Length;
            // Asientos más anchos y pasillo más angosto
            tlpSeats.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 60F)); // A
            tlpSeats.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 60F)); // B
            tlpSeats.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 30F)); // pasillo
            tlpSeats.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 60F)); // C
            tlpSeats.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 60F)); // D

            // Filas (7)
            tlpSeats.RowStyles.Clear();
            tlpSeats.RowCount = totalRows;
            for (int i = 0; i < totalRows; i++)
                tlpSeats.RowStyles.Add(new RowStyle(SizeType.Absolute, 42F)); // un poco más alto

            // Que ocupe la izquierda del form
            tlpSeats.Size = new Size(60+60+30+60+60 + 6, totalRows * 42 + (totalRows+1)); // ancho aprox + bordes
            // O si preferís que se estire:
            // tlpSeats.Dock = DockStyle.Left;

            // Crear botones
            var seatFont = new Font("Segoe UI", 9F, FontStyle.Bold);
            for (int fila = 1; fila <= totalRows; fila++)
            {
                for (int col = 0; col < seatCols.Length; col++)
                {
                    if (seatCols[col] == "") continue; // pasillo

                    string code = $"{fila}{seatCols[col]}";

                    var btn = new Button
                    {
                        Text = code,
                        Dock = DockStyle.Fill,
                        BackColor = Color.LightGray,
                        ForeColor = Color.Black,
                        Margin = new Padding(3),
                        Tag = code,
                        Font = seatFont
                    };

                    // Tooltip simple (ventana/pasillo en 2–2)
                    string tip = (seatCols[col] == "A" || seatCols[col] == "D") ? "Ventana" : "Pasillo";
                    toolTip1.SetToolTip(btn, $"Asiento {code} • {tip}");

                    btn.Click += Seat_Click;

                    tlpSeats.Controls.Add(btn, col, fila - 1);
                }
            }

            tlpSeats.ResumeLayout();
        }

        private void Seat_Click(object sender, EventArgs e)
        {
            var btn = (Button)sender;

            // Si clickeás el mismo -> deselecciona
            if (asientoSeleccionado == btn)
            {
                SetDeseleccion(btn);
                asientoSeleccionado = null;
                return;
            }

            // Si había otro seleccionado -> lo deselecciono
            if (asientoSeleccionado != null)
                SetDeseleccion(asientoSeleccionado);

            // Selecciono el nuevo
            SetSeleccion(btn);
            asientoSeleccionado = btn;
        }

        private static void SetSeleccion(Button btn)
        {
            btn.BackColor = Color.DodgerBlue;
            btn.ForeColor = Color.White;
            btn.FlatStyle = FlatStyle.Popup;
        }

        private static void SetDeseleccion(Button btn)
        {
            btn.BackColor = Color.LightGray;
            btn.ForeColor = Color.Black;
            btn.FlatStyle = FlatStyle.Standard;
        }

        public string AsientoElegido { get; private set; }

        private void btnConfirmar_Click(object sender, EventArgs e)
        {
            // Guardar el asiento seleccionado en alguna variable para usar en Form1
            if (asientoSeleccionado != null)
            {
                AsientoElegido = asientoSeleccionado.Tag.ToString();
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("Por favor, seleccione un asiento antes de confirmar.", "Atención", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void btnVolver_Click(object sender, EventArgs e)
        {
            // Volver a Form1
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

         
    }
}
