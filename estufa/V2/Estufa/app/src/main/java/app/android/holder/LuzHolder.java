package app.android.holder;

import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import app.android.R;

public class LuzHolder extends RecyclerView.ViewHolder {
    private TextView tvTituloLuz;
    private TextView tvSubLuz;

    public LuzHolder(View itemView) {
        super(itemView);
        tvTituloLuz = (TextView) itemView.findViewById(R.id.tvTituloLuz);
        tvSubLuz = (TextView) itemView.findViewById(R.id.tvSubLuz);

        final DatabaseReference database = FirebaseDatabase.getInstance().getReference().child("realtime");

        ValueEventListener listener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                int luzAcesa = dataSnapshot.child("luz").getValue(Integer.class);
                if(luzAcesa == 1) {
                    tvTituloLuz.setText("Luz acesa");
                }
                else if(luzAcesa == 0) {
                    tvTituloLuz.setText("Luz apagada");
                }

                tvSubLuz.setText("Tempo restante: " + dataSnapshot.child("cronometro_luz").getValue(String.class));
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                //tvTituloLuz.setText(databaseError.getMessage());

            }
        };
        database.addValueEventListener(listener);
    }
}
