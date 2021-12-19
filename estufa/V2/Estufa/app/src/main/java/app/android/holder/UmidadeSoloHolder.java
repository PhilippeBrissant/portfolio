package app.android.holder;

import android.view.View;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;


import java.text.DecimalFormat;

import app.android.R;

public class UmidadeSoloHolder extends RecyclerView.ViewHolder {
    private TextView tvTituloUmidadeSolo;
    private TextView tvSubUmidadeSolo;

    public UmidadeSoloHolder(View itemView) {
        super(itemView);
        tvTituloUmidadeSolo = (TextView) itemView.findViewById(R.id.tvTituloUmidade);
        tvSubUmidadeSolo = (TextView) itemView.findViewById(R.id.tvSubUmidade);
        final DatabaseReference database = FirebaseDatabase.getInstance().getReference().child("realtime");

        ValueEventListener listener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                DecimalFormat formatador = new DecimalFormat();
                formatador.setMaximumFractionDigits(2);

                int soloUmido = dataSnapshot.child("umidade_solo").getValue(Integer.class);
                //0% = 596
                //100¨= 200

                float umidade = (float) (soloUmido*100/1024 - 100)*-1;
                tvTituloUmidadeSolo.setText(formatador.format(umidade) + "%");

                int bombaLigada = dataSnapshot.child("bomba").getValue(Integer.class);
                if(bombaLigada == 1)
                    tvSubUmidadeSolo.setText("Bomba ligada");
                else if(bombaLigada == 0)
                    tvSubUmidadeSolo.setText("Bomba desligada");
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
//                tvTituloLuz.setText(databaseError.getMessage());

            }
        };
        database.addValueEventListener(listener);
    }
}
