package app.android.holder;

import android.view.View;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import app.android.R;

public class TemperaturaHolder extends RecyclerView.ViewHolder {
    private TextView tvTituloTemperatura;
    private TextView tvSubTemp;

    public TemperaturaHolder(View itemView) {
        super(itemView);
        tvTituloTemperatura = (TextView) itemView.findViewById(R.id.tvTituloTempratura);
        tvSubTemp = (TextView) itemView.findViewById(R.id.tvSubTempratura);
        final DatabaseReference database = FirebaseDatabase.getInstance().getReference().child("realtime");

        ValueEventListener listener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                tvTituloTemperatura.setText(dataSnapshot.child("temperatura").getValue(Float.class)+"ºC");

                int ventiladorLigado = dataSnapshot.child("ventilador").getValue(Integer.class);
                if(ventiladorLigado == 1)
                    tvSubTemp.setText("Ventilador ligado");
                else if(ventiladorLigado == 0)
                    tvSubTemp.setText("Ventilador desligado");
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                //tvTituloLuz.setText(databaseError.getMessage());

            }
        };

        database.addValueEventListener(listener);
    }
}
